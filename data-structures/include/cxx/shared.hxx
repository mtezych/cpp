
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


#ifndef CXX_SHARED
#define CXX_SHARED


#include <type_traits>

#include <functional>

#include <concepts>

#include <cstddef>

#include <utility>

#include <cassert>

#include <atomic>

#include <array>


namespace cxx
{
    template <std::size_t size, std::invocable function>
    //
    requires std::destructible<std::invoke_result_t<function>>
    //
    constexpr auto generate_array (function&& generator)
    //
    noexcept(std::is_nothrow_invocable_v<function>)
    //
    -> std::array<std::invoke_result_t<function>, size>
    {
        return [&] <std::size_t ... indexes> (std::index_sequence<indexes...>)
        {
            return std::array
            {
                ((void)indexes, std::invoke(std::forward<function>(generator))) ...
            };
        }
        (std::make_index_sequence<size> { });
    }


    template <std::destructible value_type>
    class shared
    {
    private:

        struct shared_state
        {
            value_type      value;
            std::atomic_int ref_count;
        };

        shared_state* state;

        explicit constexpr shared (shared_state* const state) noexcept
        :
            state { state }
        {
            assert(state != nullptr);
        }

    public:

        // note: The cxx::shared<type>::bulk_create<count>() function
        //       dynamically allocates a single value,
        //       along with its ref_count, and returns an array of handles,
        //       which are all referencing the exact same shared value.
        //
        //    +-------------------------------------------------> +-----------+
        //    |        +----------------------------------------> |   value   |
        //    |        |        +-------------------------------> | ref_count |
        //    |        |        |              +----------------> +-----------+
        //  +-|------+-|------+-|------+-----+-|------+
        //  | shared | shared | shared | ... | shared |
        //  +--------+--------+--------+-----+--------+
        //  <----------------- count ----------------->
        //
        template <int count, typename ... types> requires (count > 0)
        static constexpr
        auto bulk_create (types&& ... args) -> std::array<shared, count>
        {
            auto* const state = new shared_state
            {
                .value     = value_type(std::forward<types&&>(args)...),
                .ref_count = std::atomic_int { count }
            };

            return cxx::generate_array<count>([state] () noexcept
                                              {
                                                  return shared { state };
                                              });
        }


        // note: Only moved from objects of type cxx::shared<>
        //       can exist in the empty state.
        //
        shared () = delete;

        // [CppCon] - Hans Boehm: Using weakly ordered C++ atomics correctly
        // ~ https://www.youtube.com/watch?v=M15UKpNlpeM
        //
        // [C++ and Beyond] - Herb Sutter: atomic<> Weapons
        // ~ https://herbsutter.com/2013/02/11/atomic-weapons-the-c-memory-model-and-modern-hardware
        // ~ https://sec.ch9.ms/ch9/0b96/086be40b-912f-4cd7-a038-d09612ee0b96/CB2012SessionHerbSutterAtomicP2_mid.mp4
        // ~ https://sec.ch9.ms/ch9/aa79/4a4e75bd-46c5-4674-bc6f-34d740f9aa79/CB2012SessionHerbSutterAtomicP1_mid.mp4
        //
        // [Boost.Atomic] - Usage examples: Reference counting
        // ~ https://www.boost.org/doc/libs/1_78_0/doc/html/atomic/usage_examples.html
        //
        ~shared () noexcept
        {
            if ((state != nullptr) &&
                (state->ref_count.fetch_sub(1, std::memory_order::release) == 1))
            {
                // note : The acquire memory fence is required to ensure that
                //        all operations on the shared value will happen-before
                //        that shared value will be destroyed.
                //
                //        Relying on data-flow / control-flow dependencies
                //        to establish ordering is not sufficient,
                //        since CPUs can perform speculative execution.
                //
                std::atomic_thread_fence(std::memory_order::acquire);

                delete state;
            }
        }


        // note: To limit the spread of sharing possibly mutable values,
        //       the cxx::shared<> doesn't allow copying of handles,
        //       which combined with cxx::shared::bulk_create<count>()
        //       guarantees at compile-time, that there will exist at most
        //       only count number of handles, sharing the exact same value.
        //
        shared (const shared&) = delete;

        auto operator = (const shared&) -> shared& = delete;


        // note: It is assumed that a moved from object of type cxx::shared<>,
        //       can be either destroyed or a new value can be assigned to it.
        //
        constexpr
        shared (shared&& other) noexcept
        :
            state { other.state }
        {
            other.state = nullptr;
        }

        auto operator = (shared&& other) noexcept -> shared&
        {
            if ((state != nullptr) &&
                (state->ref_count.fetch_sub(1, std::memory_order::release) == 1))
            {
                std::atomic_thread_fence(std::memory_order::acquire);

                delete state;
            }

            state = other.state;
            other.state = nullptr;

            return *this;
        }


        // todo: Conversions from cxx::shared<type> to cxx::shared<const type>
        //       could be allowed, to model raw & smart pointers more closely.
        //
        constexpr auto operator == (const shared& other) const noexcept -> bool
        {
            return this->state == other.state;
        }


        constexpr auto operator * () const noexcept -> value_type&
        {
            return state->value;
        }

        constexpr auto operator -> () const noexcept -> value_type*
        {
            return &state->value;
        }
    };
}


#endif
