
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


#include <cxx/channel.hxx>

#include <cstdlib>

#include <vector>

#include <thread>

#include <span>

#include <string>

#include <iostream>


namespace demo
{
    template <std::invocable function_type>
    requires std::is_copy_constructible_v<function_type>
    //
    auto async_invoke (std::vector<std::jthread>& threads,
                       std::span<function_type> functions) -> cxx::channel::receiver
    {
        auto [receiver, sender_generator] = cxx::channel
                                               ::create(std::ssize(functions));

        for (const auto& function : functions)
        {
            threads.emplace_back([sender = sender_generator(), function] () mutable
                                 {
                                     auto signal = cxx::channel::scoped_signal
                                                   {
                                                       std::move(sender)
                                                   };

                                    std::invoke(function);
                                 });
        }

        return std::move(receiver);
    }
}


auto main () -> int
{
    auto message   = std::string { };
    auto answer    = int         { };

    auto threads   = std::vector<std::jthread> { };

    auto functions = std::vector<std::function<auto () -> void>>
                     {
                         [&] { message = "Hello, from async_invoke()! \n"; },
                         [&] { answer  = 42;                               },
                     };

    auto receiver  = demo::async_invoke(threads, std::span { functions });

    cxx::channel::wait(receiver);

    auto function  = [&] { std::cout << message
                                     << "The answer is " << answer << "!\n"; };

    demo::async_invoke(threads, std::span { &function, 1 });

    return EXIT_SUCCESS;
}
