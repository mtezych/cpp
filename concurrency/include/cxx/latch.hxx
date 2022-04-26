
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Mateusz Zych
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


#include <cstddef>

#include <atomic>

#include <limits>

#include <cxx/atomic_wait.hxx>


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
    //
    // [CppCon] - Hans Boehm: Using weakly ordered C++ atomics correctly
    // ~ https://www.youtube.com/watch?v=M15UKpNlpeM
    //
    // [CppCon] - Fedor Pikus: C++ atomics, from basic to advanced.
    //                         What do they really do?
    // ~ https://www.youtube.com/watch?v=ZQFzMfHIxng
    //
    // [CppCon] - Bryce Adelstein Lelbach: The C++ Execution Model
    // ~ https://www.youtube.com/watch?v=FJIn1YhPJJc
    //
    // [ModernesCpp] - Rainer Grimm: Acquire-Release Fences
    // ~ https://www.modernescpp.com/index.php/acquire-release-fences


    class latch
    {
    private:
        std::atomic<std::ptrdiff_t> counter;

    public:
        static constexpr auto max () noexcept -> std::ptrdiff_t
        {
            return std::numeric_limits<std::ptrdiff_t>::max();
        }

        constexpr explicit latch (const std::ptrdiff_t initial_value) noexcept
        :
            counter { initial_value }
        { }

        constexpr ~latch () noexcept = default;

        latch (const latch&) = delete;

        auto operator = (const latch&) -> latch& = delete;

        // note: The cxx::latch::arrive() member function is identical
        //       to the std::latch::count_down(), except:
        //
        //       - it is named arrive() instead of count_down(),
        //         to better reflect its usage instead of its implementation
        //
        //       - returns state of the latch (open or closed)
        //         ~ false -> latch is still closed
        //         ~ true  -> latch has just been opened
        //
        //       Only the last invocation of the cxx::latch::arrive()
        //       can change the state of a cxx::latch from closed to open and
        //       return true, since it is undefined behaviour
        //       to call cxx::latch::arrive() on an already opened cxx::latch.
        //
        //       This enables a client to perform an action exactly once,
        //       right after the cxx::latch has been opened,
        //       in which all prior operations,
        //       that happened-before any invocation of the cxx::latch::arrive(),
        //       are guaranteed be visible:
        //
        //       // thread A           // thread B
        //       x = 10;               y = 11;
        //       if (latch.arrive())   if (latch.arrive())
        //       {                     {                   //     assignments to
        //         assert(x == 10);      assert(x == 10);  // <-- both x and y
        //         assert(y == 11);      assert(y == 11);  //     are guaranteed
        //       }                     }                   //     to be visible
        //
        auto arrive (const std::ptrdiff_t update = 1) noexcept -> bool
        {
            const auto old_value = counter.fetch_sub(update,
                                                     std::memory_order::release);

            if (old_value == update) // counter reached 0
            {
                // note: In order to guarantee visibility of all prior operations,
                //       that happened-before invoking the cxx::latch::arrive(),
                //       potentially in multiple different C++ threads,
                //       a memory fence performing an acquire operation is required.
                //
                //       Moreover, without that memory fence an action
                //       performed in response to the opening of the cxx::latch,
                //       could actually be speculatively executed
                //       before the cxx::latch has been opened:
                //       if (latch.arrive()) // branch predictor guesses true
                //       { action(); }      // action() executes before arrive()
                //
                //       Interestingly, the std::latch::count_down()
                //       does not require an acquire operation, since:
                //
                //       - it returns void, which effectively forces a client
                //         to obtain the state of the std::latch using either
                //         std::latch::try_wait() or std::latch::wait(),
                //         which themselves perform an acquire operation
                //
                //       - std::atomic::fetch_sub() and std::atomic::notify_all()
                //         are both atomic operations, which are guaranteed to
                //         never be reordered within a single thread,
                //         as long as they operate on the same atomic value,
                //         even when the relaxed memory ordering is used
                //
                std::atomic_thread_fence(std::memory_order::acquire);

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
            const auto
            current_value = counter.fetch_sub(update,
                                              std::memory_order::release) - update;

            if (current_value == std::ptrdiff_t { 0 })
            {
                std::atomic_thread_fence(std::memory_order::acquire);

                counter.notify_all();
            }
            else // waiting is required, since counter have not reached 0 yet
            {
                counter.wait(current_value, std::memory_order::relaxed);

                cxx::atomic_wait(counter, std::ptrdiff_t { 0 });
            }
        }
    };
}


#endif
