
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


#include <cxx/spin_mutex.hxx>

#include <catch2/catch.hpp>

#include <array>
#include <mutex>
#include <thread>


TEST_CASE ("[spin_mutex] smoke test")
{
    auto counter = std::atomic<int> { 0 };
    {
        auto mutex = cxx::spin_mutex { };

        auto thread_main = [&] () -> void
        {
            auto lock = std::scoped_lock { mutex };

            // note: At any point in time,
            //       only one thread is allowed in the critical section.

            REQUIRE(counter++ == 0);
            {
                std::this_thread::sleep_for(std::chrono::microseconds { 100 });
            }
            REQUIRE(counter-- == 1);
        };

        auto threads = std::array
                       {
                           std::jthread { thread_main },
                           std::jthread { thread_main },
                       };
    }
    REQUIRE(counter == 0);
}
