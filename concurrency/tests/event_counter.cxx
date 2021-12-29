
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


#include <type_traits>

#include <concepts>

#include <cstddef>

#include <array>

#include <utility>


namespace cxx
{
    template <typename type, typename ... types>
    concept nothrow_constructible_from = bool
    {
        std::is_nothrow_constructible_v<type, types...> &&
        std:: is_nothrow_destructible_v<type>
    };


    template <typename element, std::size_t size, typename ... types>
    /* */
    constexpr auto create_filled_array (types&& ... args)
    /* */
    noexcept (cxx::nothrow_constructible_from<element, types&&...>)
    /* */                                           -> std::array<element, size>
    requires (std::        constructible_from<element, types&&...>)
    {
        return [&] <std::size_t ... indexes> (std::index_sequence<indexes...>)
        {
            return std::array
            {
                ((void)indexes, element(std::forward<types>(args) ...)) ...
            };
        }
        (std::make_index_sequence<size> { });
    }
}


#include <cxx/event_counter.hxx>

#include <catch2/catch.hpp>

#include <thread>


TEST_CASE ("[event_counter] synchronize threads")
{
    bool result; // uninitialized aka partially-formed
    {
        constexpr
        auto thread_count  = 4;

        auto event_counter = cxx::event_counter { thread_count };

        auto shared_value  = std::atomic<int> { 0 };

        auto thread_main   = [&] () -> void
        {
            shared_value.fetch_add(1, std::memory_order::relaxed);

            if (event_counter.count_down())
            {
                result =
                (shared_value.load(std::memory_order::relaxed) == thread_count);
            }
        };

        [[maybe_unused]]
        auto threads =
             cxx::create_filled_array<std::jthread, thread_count>(thread_main);
    }
    REQUIRE(result == true);
}
