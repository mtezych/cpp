
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


#include <cxx/chunk_evenly.hxx>

#include <catch2/catch.hpp>

#include <functional>

#include <algorithm>

#include <iterator>

#include <vector>

#include <array>


namespace
{
    constexpr
    auto equal_chunks = [] (std::ranges::input_range auto&&  left_chunk,
                            std::ranges::input_range auto&& right_chunk) -> bool
    requires
    std::indirectly_comparable<std::ranges::iterator_t<decltype( left_chunk)>,
                               std::ranges::iterator_t<decltype(right_chunk)>,
                               std::ranges::equal_to>
    {
        return std::ranges::equal(left_chunk, right_chunk);
    };


    [[nodiscard]]
    constexpr auto to_std_vector (std::ranges::input_range auto&& range)
    //
    requires std::copyable<std::ranges::range_value_t<decltype(range)>>
    {
        return std::vector(std::ranges::begin(range), std::ranges::end(range));
    }


    template <typename range>  requires std::ranges:: forward_range<range> &&
                                        std::ranges::   sized_range<range> &&
                                        std::ranges::viewable_range<range>
    [[nodiscard]]
    constexpr auto is_range_chunked_evenly (range&& range_of_chunks) -> bool
    //
    requires std::ranges::forward_range<std::ranges::range_reference_t<range>> &&
             std::              regular<std::ranges::range_value_t    <range>>
    {
        const
        auto range_of_elements = to_std_vector(range_of_chunks | std::views::join);

        const auto chunk_count = std::ranges::ssize(range_of_chunks);

        return
        std::ranges::equal(range_of_elements | cxx::chunk_evenly(chunk_count),
                           range_of_chunks,
                           equal_chunks);
    }
}

TEST_CASE ("equally-sized chunks", "[chunk_evenly]")
{
    const auto chunked_range = std::array
                               {
                                   std::vector { '@', '#', '%' },
                                   std::vector { '$', '^', '&' },
                               };

    REQUIRE(is_range_chunked_evenly(chunked_range));
}

TEST_CASE ("unequally-sized chunks", "[chunk_evenly]")
{
    const auto chunked_range = std::array
                               {
                                   std::vector { '_', '"', '|' },
                                   std::vector { '=', '+', '-' },
                                   std::vector { '~', '*', '/' },
                                   std::vector { '!', '?' },
                                   std::vector { ':', ';' },
                                   std::vector { '.', ',' },
                                   std::vector { '>', '<' },
                               };

    REQUIRE(is_range_chunked_evenly(chunked_range));
}

TEST_CASE ("single empty chunk", "[chunk_evenly]")
{
    const auto chunked_range = std::array
                               {
                                   std::vector { '|' },
                                   std::vector { '|' },
                                   std::vector { '|' },
                                   std::vector<char>{},
                               };

    REQUIRE(is_range_chunked_evenly(chunked_range));
}


TEST_CASE ("multiple empty chunks", "[chunk_evenly]")
{
    const auto chunked_range = std::array
                               {
                                   std::vector { '|' },
                                   std::vector<char>{},
                                   std::vector<char>{},
                                   std::vector<char>{},
                               };

    REQUIRE(is_range_chunked_evenly(chunked_range));
}


TEST_CASE ("only empty chunks", "[chunk_evenly]")
{
    const auto chunked_range = std::array
                               {
                                   std::vector<char>{},
                                   std::vector<char>{},
                                   std::vector<char>{},
                               };

    REQUIRE(is_range_chunked_evenly(chunked_range));
}


TEST_CASE ("only single empty chunk", "[chunk_evenly]")
{
    const auto chunked_range = std::array
                               {
                                   std::vector<char>{},
                               };

    REQUIRE(is_range_chunked_evenly(chunked_range));
}


TEST_CASE ("unequally-sized chunks must be grouped together", "[chunk_evenly]")
{
    const auto chunked_range = std::array
                               {
                                   std::vector { '>', '<' },
                                   std::vector { '~' },
                                   std::vector { '(', ')' },
                                   std::vector { '[', ']' },
                                   std::vector { '|' },
                               };

    REQUIRE(!is_range_chunked_evenly(chunked_range));
}


TEST_CASE ("group of larger chunks must be at the front", "[chunk_evenly]")
{
    const auto chunked_range = std::array
                               {
                                   std::vector { '|' },
                                   std::vector { '~' },
                                   std::vector { '(', ')' },
                                   std::vector { '[', ']' },
                               };

    REQUIRE(!is_range_chunked_evenly(chunked_range));
}


TEST_CASE ("evaluate at compile-time", "[chunk_evenly]")
{
    constexpr auto chunked_range = std::array
                                   {
                                       std::array { 0, 1, 2, 3 },
                                       std::array { 4, 5, 6, 7 },
                                   };

    static_assert(is_range_chunked_evenly(chunked_range));
}


TEST_CASE ("compose with std range factories and adaptors", "[chunk_evenly]")
{
    constexpr auto chunked_range = std::array
                                   {
                                       std::array { 0, 1, 2 },
                                       std::array { 3, 4, 5 },
                                   };

    const auto range_of_chunks = std::views::iota(0, 8) | cxx::chunk_evenly(3) |
                                 std::views::take(2);

    REQUIRE(std::ranges::equal(range_of_chunks, chunked_range, equal_chunks));
}


TEST_CASE ("create pipeline", "[chunk_evenly]")
{
    auto  pipeline      = cxx::chunk_evenly(3) | cxx::chunk_evenly(2);

    using pipeline_type = cxx::ranges::pipeline
                          <
                              cxx::chunk_evenly_closure<int>,
                              cxx::chunk_evenly_closure<int>
                          >;

    static_assert(std::same_as<decltype(pipeline), pipeline_type>);
}
