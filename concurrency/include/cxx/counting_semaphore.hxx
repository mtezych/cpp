
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

#ifndef CXX_COUNTING_SEMAPHORE
#define CXX_COUNTING_SEMAPHORE


#include <cstddef>

#include <limits>

#include <atomic>

#include <cassert>

#include <concepts>


namespace cxx
{
    // [GitHub] - Olivier Giroux: C++20 synchronization facilities
    // ~ https://github.com/ogiroux/atomic_wait/blob/master/include/semaphore
    //
    // [GitHub] - Microsoft: STL
    // ~ https://github.com/microsoft/STL/blob/main/stl/inc/semaphore
    //
    // [GitHub] - LLVM: Clang libc++
    // ~ https://github.com/llvm/llvm-project/blob/main/libcxx/include/semaphore
    //
    // [GCC Git] - GNU: GCC libstdc++
    // ~ https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/include/bits/semaphore_base.h;hb=refs/heads/trunk
    // ~ https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/include/std/semaphore;hb=refs/heads/trunk
    //
    // [C++ Russia] - Bryce Adelstein Lelbach: The C++20 synchronization library
    // ~ https://www.youtube.com/watch?v=z7fPxjZKsBY


    template <std::integral integral>
    class counting_semaphore
    {
    private:
        std::atomic<integral> counter;

    public:
        static constexpr auto max () noexcept -> std::ptrdiff_t
        {
            return std::numeric_limits<integral>::max();
        }

        explicit counting_semaphore (const integral desired) noexcept
        :
            counter { desired }
        {
            assert((desired >= 0) && (desired <= max()));
        }

        ~counting_semaphore () = default;

        counting_semaphore (const counting_semaphore&)                 = delete;
        auto  operator  =  (const counting_semaphore&) ->
                                                   counting_semaphore& = delete;

        auto release (const integral update = 1) noexcept -> void
        {
            if (counter.fetch_add(update, std::memory_order::release) == 0)
            {
                if (update > 1) { counter.notify_all(); }
                else            { counter.notify_one(); }
            }
        }

        auto acquire () noexcept -> void
        {
            const auto
            try_acquire_until_cannot_enter_critical_section = [this] () noexcept
                                                                        ->  bool
            {
                auto old_value = counter.load(std::memory_order::acquire);

                while (old_value != 0)
                {
                    assert(old_value > 0);

                    if (counter.compare_exchange_weak(old_value, old_value - 1,
                                                      std::memory_order::acquire,
                                                      std::memory_order::relaxed))
                    {
                        return true;
                    }
                }

                return false;
            };

            while (!try_acquire_until_cannot_enter_critical_section())
            {
                counter.wait(0, std::memory_order::relaxed);
            }
        }

        auto try_acquire () noexcept -> bool
        {
            auto old_value = counter.load(std::memory_order::acquire);

            if (old_value == 0)
            {
                return false;
            }
            else
            {
                assert(old_value > 0);

                return
                counter.compare_exchange_strong(old_value, old_value - 1,
                                                std::memory_order::acquire,
                                                std::memory_order::relaxed);
            }
        }
    };
}


#endif
