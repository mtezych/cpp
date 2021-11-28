
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


#ifndef CXX_LATCH
#define CXX_LATCH


#include <concepts>

#include <cstddef>

#include <atomic>

#include <limits>


namespace cxx
{
    // [C++20] P1135R6: The C++20 Synchronization Library
    // ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1135r6.html
    //
    // [C++20] P1865R1: Add max() to latch and barrier
    // ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1865r1.html
    //
    // [C++ reference] - std::atomic_wait()
    // ~ https://en.cppreference.com/w/cpp/atomic/atomic_wait
    //
    // [C++ reference] - std::latch::wait()
    // ~ https://en.cppreference.com/w/cpp/thread/latch/wait
    //
    // [Wikipedia] - Spurious wakeup
    // ~ https://en.wikipedia.org/wiki/Spurious_wakeup
    //
    // [GitHub] - LLVM: Clang libc++
    // ~ https://github.com/llvm/llvm-project/blob/main/libcxx/include/latch
    //
    // [GCC Git] - GNU: GCC libstdc++
    // ~ https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/include/std/latch;hb=refs/heads/trunk
    //
    // [GitHub] - Microsoft: STL
    // ~ https://github.com/microsoft/STL/blob/main/stl/inc/latch
    //
    // [GitHub] - Olivier Giroux: C++20 synchronization facilities
    // ~ https://github.com/ogiroux/atomic_wait/blob/master/include/latch


    // note: The C++ standard library
    //       does *not* provide std::atomic_wait() overload,
    //       which waits until a predicate is satisfied or
    //       waits until value of std::atomic changes to the new value.
    //
    template <std::integral integral_type>
    auto atomic_wait (const std::atomic<integral_type>& atomic,
                                        integral_type new_value) noexcept -> void
    {
        // note: Even though std::atomic::wait(old_value) handles spurious wakeups,
        //       it may still wake up (stop waiting), due to spurious wakeup
        //       *and* a change from the old_value to some different value,
        //       which might not necessarily be equal to the new_value.
        //
        //       To handle the above case, the loop below will
        //       load current_value of the atomic in the next iteration
        //       and wake up only when the current_value equals to the new_value.
        //
        while (true)
        {
            const auto current_value = atomic.load(std::memory_order::acquire);

            if (current_value == new_value)
            {
                return;
            }
            else
            {
                // note: The cxx::atomic_wait() *always* ends on loading
                //       value of the atomic counter with acquire memory_order,
                //       thus waiting with relaxed memory_order is sufficient.
                //
                atomic.wait(current_value, std::memory_order::relaxed);
            }
        }
    }


    class latch
    {
    private:
        std::atomic<std::ptrdiff_t> counter;

    public:
        static constexpr auto max () noexcept -> std::ptrdiff_t
        {
            return std::numeric_limits<int>::max();
        }

        constexpr explicit latch (const std::ptrdiff_t initial_value) noexcept
        :
            counter { initial_value }
        { }

        constexpr ~latch() noexcept = default;

        latch (const latch&) = delete;

        auto operator = (const latch&) -> latch& = delete;

        auto arrive (const std::ptrdiff_t update = 1) noexcept -> bool
        {
            const auto old_value = counter.fetch_sub(update,
                                                     std::memory_order::release);

            if (old_value == update) // counter reached 0
            {
                counter.notify_all();

                return true;
            }
            else
            {
                return false;
            }
        }

        [[nodiscard]]
        auto try_wait () const noexcept -> bool
        {
            return counter.load(std::memory_order::acquire) == 0;
        }

        auto wait () const noexcept -> void
        {
            cxx::atomic_wait(counter, std::ptrdiff_t { 0 });
        }

        auto arrive_and_wait (const std::ptrdiff_t update = 1) noexcept -> void
        {
            arrive(update);

            wait();
        }
    };
}


#endif
