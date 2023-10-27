
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2023, Mateusz Zych
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


#ifndef CXX_SPIN_MUTEX
#define CXX_SPIN_MUTEX


#include <atomic>


namespace cxx
{
    // [C++ Russia] Bryce Adelstein Lelbach - The C++20 synchronization library
    //
    // ~ https://www.youtube.com/watch?v=z7fPxjZKsBY

    // [CppCon 2019] Olivier Giroux - Putting std::atomic in CUDA
    //
    // ~ https://www.youtube.com/watch?v=VogqOscJYvk

    // [P0514R4] Olivier Giroux - Efficient concurrent waiting for C++20
    //
    // ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0514r4.pdf

    // [NVIDIA] - CUDA C++ Programming Guide: Nanosleep Function
    //
    // ~ https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#id39

    // [C++ reference] - std::mutex
    //
    // ~ https://en.cppreference.com/w/cpp/thread/mutex

    // [C++ reference] - std::atomic_flag
    //
    // ~ https://en.cppreference.com/w/cpp/atomic/atomic_flag

    class spin_mutex
    {
    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT; // note: initialize to false

    public:
        auto lock () noexcept -> void
        {
            // note: To try to lock the spin_mutex,
            //       a thread unconditionally sets the atomic_flag to true.
            //
            //       Consider the following scenarios:
            //
            //       - if the previous value was false,
            //         the spin_mutex was previously unlocked,
            //         and it has been successfully locked,
            //         thus the while loops ends
            //
            //       - if the previous value was true,
            //         the spin_mutex is currently locked by another thread,
            //         thus the while loops continues and
            //         the thread trying to lock the spin_mutex
            //         goes to sleep until it will be woken up by
            //         the thread that has locked the spin_mutex
            //
            //       The scenario in which,
            //       a thread that has already locked a spin_mutex,
            //       attempts to lock it again is considered
            //       an undefined behaviour (a programming error),
            //       and therefore does not need to be considered.
            //
            while (flag.test_and_set(std::memory_order_acquire))
            {
                flag.wait(true, std::memory_order_relaxed);
            }
        }

        auto try_lock () noexcept -> bool
        {
            return !flag.test_and_set(std::memory_order_acquire);
        }

        auto unlock () noexcept -> void
        {
            // note: To unlock the spin_mutex
            //       a thread sets the atomic_flag to false, and then
            //
            //       notifies one of the threads
            //       waiting to lock the spin_mutex to wake up,
            //       assuming such a thread exists.
            //
            flag.clear(std::memory_order_release);
            flag.notify_one();
            //
            // note: Even though, the above .notify_one()
            //       will not wake up a thread
            //       in the case when another thread will call .test_and_set()
            //       between .clear() and .notify_one() calls,
            //
            //       the list of threads waiting to lock the spin_mutex
            //       will not experience an unlimited growth, because:
            //
            //        - the .try_lock() does never put any threads to sleep,
            //          that is, it never increases the length of that list
            //
            //        - the .lock() is also guaranteed to do not put to sleep
            //          the thread that was trying to lock the spin_mutex,
            //          since in this case the while loop
            //          in the .lock() member function will end immediately,
            //          due to the .test_and_set() call returning false,
            //          that is, the previous value of the atomic_flag
            //          established by the .clear() call
            //          at the beginning of the .unlock() member function.
        }
    };
}


#endif
