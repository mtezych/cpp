
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2021, Mateusz Zych
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef CXX_CHANNEL
#define CXX_CHANNEL


#include <cxx/latch.hxx>

#include <cxx/tuple.hxx>

#include <concepts>

#include <functional>

#include <cstddef>

#include <atomic>

#include <cassert>


#if   defined(_MSC_VER)

    // [MSVC] - __assume
    // - https://docs.microsoft.com/en-us/cpp/intrinsics/assume
    #define cxx_assume(condition) __assume(condition)

#elif defined(__clang__)

    // [Clang] - Language Extensions
    // - https://clang.llvm.org/docs/LanguageExtensions.html#builtin-assume
    #define cxx_assume(condition) __builtin_assume(condition)

#elif defined(__GNUC__)

    // [GCC] - Other Built-in Functions
    // - https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
    #define cxx_assume(condition) ((condition) ? void() : __builtin_unreachable())

#else
    static_assert(false, "C++ compiler is not supported!");
#endif


#ifdef NDEBUG
    #define cxx_expects(condition) cxx_assume(condition)
#else
    #define cxx_expects(condition)     assert(condition)
#endif


namespace cxx
{
    // [WG21] P2401R0: Add a conditional noexcept specification to std::exchange
    // ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2401r0.html
    //
    // [WG21] N3668: std::exchange() utility function, revision 3
    // ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3668.html
    //
    // todo: Remove cxx::exchange() after enabling C++23,
    //       by replacing its usages with calls to std::exchange().
    //
    template <typename object_type, typename value_type = object_type>
    //
    constexpr auto exchange (object_type& object, value_type&& new_value)
    //
    noexcept(std::is_nothrow_assignable_v<object_type&, value_type> &&
             std::is_nothrow_move_constructible_v<object_type>)  ->  object_type
    {
        auto old_value = std::move(object);

        object = std::forward<value_type>(new_value);

        return old_value;
    }


    template <std::invocable generator_type>
    class counted_generator
    {
    private:

        std::ptrdiff_t      index;
        generator_type  generator;

    public:

        explicit constexpr counted_generator (const std::ptrdiff_t     count,
                                              generator_type       generator)
        //
        noexcept (std::is_nothrow_move_constructible_v<generator_type>)
        requires (std::        is_move_constructible_v<generator_type>)
        :
            index     {             count    },
            generator { std::move(generator) }
        { }

        constexpr ~counted_generator () noexcept
        requires std::destructible<generator_type>
        {
            cxx_expects(index == 0);
        }


        counted_generator (const counted_generator&) = delete;

        auto operator = (const counted_generator&) -> counted_generator& = delete;


        constexpr auto operator () ()
        noexcept (std::is_nothrow_invocable_v<generator_type>)
                                      -> std::invoke_result_t<generator_type>
        {
            cxx_expects(index > 0);

            return --index, std::invoke(generator);
        }
    };
}


namespace cxx::channel // multi-producer single-consumer
{
    class sender
    {
    private:

        std::atomic<std::ptrdiff_t>* ref_count;

        explicit constexpr
        sender (std::atomic<std::ptrdiff_t>* const ref_count) noexcept
        :
            ref_count { ref_count }
        {
            cxx_expects(ref_count != nullptr);
        }

    public:

        sender () = delete;

        ~sender () noexcept
        {
            cxx_expects(ref_count == nullptr);
        }


        sender (const sender&) = delete;

        auto operator = (const sender&) -> sender& = delete;


        constexpr sender (sender&& other) noexcept
        :
            ref_count { other.ref_count }
        {
            other.ref_count = nullptr;
        }

        auto operator = (sender&& other) noexcept -> sender&
        {
            cxx_expects(ref_count == nullptr);

            ref_count = other.ref_count;
            other.ref_count = nullptr;

            return *this;
        }


        constexpr auto operator == (const sender& other) const noexcept -> bool
        {
            return this->ref_count == other.ref_count;
        }


        friend auto signal (cxx::channel::sender sender) noexcept -> void
        {
            const auto reference_count = cxx::exchange(sender.ref_count, nullptr);

            switch (reference_count->fetch_sub(1, std::memory_order::release))
            {
                case 2: // either receiver or one other sender is still alive
                {
                    // note: There are two scenarios covered by this case:
                    //
                    //       1. receiver is alive and all other senders are dead
                    //
                    //          In this scenario std::atomic::notify_all()
                    //          wakes up all threads waiting for
                    //          all signals to be sent to the channel.
                    //
                    //       2. one other sender is alive and receiver is dead
                    //
                    //          Even though *not* all signals has been sent
                    //          std::atomic::notify_all() is still called.
                    //
                    //          However, doing so is correct, because
                    //          the receiver has already been destroyed,
                    //          thus no thread is waiting for signals to arrive.
                    //
                    //          This effectively makes the invocation of
                    //          std::atomic::notify_all() a NOP (no operation).
                    //
                    reference_count->notify_all();
                    //
                    // note: C++ memory fence performing an acquire operation
                    //       is not required at all, because:
                    //
                    //       - a client cannot take an action based on whether
                    //         all signals have been sent to the channel
                    //         without calling cxx::channel::wait() first,
                    //         since cxx::channel::signal() returns void,
                    //         which effectively forces a client to perform
                    //         an acquire operation, which will ensure
                    //         visibility of operations, that happened-before
                    //         each signal had been sent to the channel
                    //
                    //       - it is guaranteed that atomic operations,
                    //         such as fetch_sub() and notify_all(),
                    //         will not be reordered within a single thread,
                    //         as long as they operate on the same atomic value,
                    //         even when using relaxed memory ordering

                    break;
                }
                case 1: // receiver and all other senders are already dead
                {
                    std::atomic_thread_fence(std::memory_order::acquire);

                    delete reference_count;

                    break;
                }
            }
        }


        friend auto create (std::ptrdiff_t capacity);
    };


    auto signal (cxx::channel::sender sender) noexcept -> void;


    class receiver
    {
    private:

        std::atomic<std::ptrdiff_t>* ref_count;

        explicit constexpr
        receiver (std::atomic<std::ptrdiff_t>* const ref_count) noexcept
        :
            ref_count { ref_count }
        {
            cxx_expects(ref_count != nullptr);
        }

    public:

        receiver () = delete;

        ~receiver () noexcept
        {
            if ((ref_count != nullptr) &&
                (ref_count->fetch_sub(1, std::memory_order::release) == 1))
            {
                std::atomic_thread_fence(std::memory_order::acquire);

                delete ref_count;
            }
        }


        receiver (const receiver&) = delete;

        auto operator = (const receiver&) -> receiver& = delete;


        constexpr receiver (receiver&& other) noexcept
        :
            ref_count { other.ref_count }
        {
            other.ref_count = nullptr;
        }

        auto operator = (receiver&& other) noexcept -> receiver&
        {
            if ((ref_count != nullptr) &&
                (ref_count->fetch_sub(1, std::memory_order::release) == 1))
            {
                std::atomic_thread_fence(std::memory_order::acquire);

                delete ref_count;
            }

            ref_count = other.ref_count;
            other.ref_count = nullptr;

            return *this;
        }


        constexpr auto operator == (const receiver& other) const noexcept -> bool
        {
            return this->ref_count == other.ref_count;
        }


        friend auto wait (const channel::receiver& receiver) noexcept -> void
        {
            // note: When the ref_count will reach 1:
            //       - only this one receiver will be alive
            //       - all signals will have been sent to the channel,
            //         because all senders will be already destroyed
            //
            //       Since the mere existence of this receiver
            //       ensures that the ref_count will be >= 1,
            //       when the ref_count will reach 1,
            //       it will remain 1 until destruction of this receiver.
            //
            cxx::atomic_wait(*receiver.ref_count, std::ptrdiff_t { 1 });
        }


        friend auto create (std::ptrdiff_t capacity);
    };


    auto wait (const channel::receiver& receiver) noexcept -> void;


    auto create (const std::ptrdiff_t capacity)
    {
        auto* const ref_count = new std::atomic<std::ptrdiff_t>
                                    {
                                        1 + capacity
                                    };
        const
        auto create_sender = [=] { return channel::sender { ref_count }; };

        return cxx::tuple
               {
                   channel::receiver        {               ref_count },
                   cxx::counted_generator   { capacity, create_sender },
               };
    }


    class scoped_signal
    {
    private:

        channel::sender sender;

    public:

        explicit scoped_signal (channel::sender sender) noexcept
        :
            sender { std::move(sender) }
        { }

        ~scoped_signal () noexcept
        {
            channel::signal(std::move(sender));
        }


        scoped_signal (const scoped_signal&) = delete;

        auto operator = (const scoped_signal&) -> scoped_signal& = delete;
    };
}


#endif
