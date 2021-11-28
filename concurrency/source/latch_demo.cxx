
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


#include <cxx/latch.hxx>

#include <concepts>

#include <cstddef>

#include <random>

#include <iostream>

#include <array>

#include <atomic>

#include <thread>

#include <numeric>

#include <functional>


namespace demo
{
    template <std::floating_point floating_point>
    auto generate_random () -> floating_point
    {
        auto random_device = std::random_device { };

        auto distribution  = std::uniform_real_distribution<floating_point>
                             {
                                 0.0f, 1.0f
                             };

        return distribution(random_device);
    }


    template <std::destructible type, std::size_t size>
    auto operator << (std::ostream& output_stream,
                      const std::array<type, size>& array) -> std::ostream&
    {
        output_stream << '[';

        for (auto& element : array)
        {
            output_stream << ' ' << element;
        }

        return output_stream << ' ' << ']';
    }
}


auto main () -> int
{
    auto values = std::array<float, 8> { };

    auto latch  = cxx::latch       { std::ssize(values) };
    auto flag   = std::atomic_flag {      false         };

    auto worker_main = [&] (const int worker_index) -> void
                       {
                           values[worker_index] = demo::generate_random<float>();

                           if (latch.arrive())
                           {
                               using demo::operator <<;

                               std::cout << values << '\n';

                               flag.test_and_set();
                               flag.notify_one();
                           }
                       };

    auto workers = std::array
    {
        std::jthread { worker_main, 0 }, std::jthread { worker_main, 4 },
        std::jthread { worker_main, 1 }, std::jthread { worker_main, 5 },
        std::jthread { worker_main, 2 }, std::jthread { worker_main, 6 },
        std::jthread { worker_main, 3 }, std::jthread { worker_main, 7 },
    };

    latch.wait();

    const auto sum = std::reduce(std::begin(values),
                                 std:: end (values), 0.0f, std::plus { });

    flag.wait(false);

    std::cout << sum << '\n';

    return EXIT_SUCCESS;
}
