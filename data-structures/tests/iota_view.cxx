
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


#include <cxx/iota_view.hxx>

#include <catch2/catch.hpp>

#include <concepts>

#include <iterator>

#include <ranges>

#include <limits>

#include <random>

#include <cassert>


TEST_CASE("cxx::iota_view models a range", "[iota_view]")
{
    auto iota_view = cxx::iota_view { 0, 10 };

    static_assert(std::ranges::range<decltype(iota_view)>);

    REQUIRE(std::        begin(iota_view) == std::        begin(iota_view));
    REQUIRE(std::ranges::begin(iota_view) == std::ranges::begin(iota_view));

    REQUIRE(std::        begin(iota_view) != std::          end(iota_view));
    REQUIRE(std::ranges::begin(iota_view) != std::ranges::  end(iota_view));
}


namespace
{
    template <std::ranges::input_range input_range>
    constexpr
    auto in_range (const                 input_range& range,
        const std::ranges::range_value_t<input_range> start,
        const std::ranges::range_value_t<input_range> stop,
        const std::ranges::range_value_t<input_range> step = 1) noexcept -> bool
    requires
        std::integral<std::ranges::range_value_t<input_range>>
    {
        auto value = start;

        for (auto element : range)
        {
            if (element != value) { return false; }

            value += step;
        }

        if (value != stop) { return false; } else { return true; }
    }
}


TEST_CASE("cxx::iota_view models an input_range", "[iota_view]")
{
    auto iota_view = cxx::iota_view { -7, +9 };

    static_assert(std::ranges::input_range<decltype(iota_view)>);

    REQUIRE(in_range(iota_view, -7, +9));
}


TEST_CASE("cxx::iota_view models a forward_range", "[iota_view]")
{
    auto iota_view = cxx::iota_view { -7, +9 };

    static_assert(std::ranges::forward_range<decltype(iota_view)>);

    REQUIRE(*                  std::        begin(iota_view)  == -7);
    REQUIRE(*                  std::ranges::begin(iota_view)  == -7);
    REQUIRE(*std::ranges::next(std::        begin(iota_view)) == -6);
    REQUIRE(*std::ranges::next(std::ranges::begin(iota_view)) == -6);

    for (auto index = 0; index != 10; ++index)
    {
        REQUIRE(in_range(iota_view, -7, +9));
    }
}


TEST_CASE("cxx::iota_view models a bidirectional_range", "[iota_view]")
{
    auto iota_view = cxx::iota_view { -7, +9 };

    static_assert(std::ranges::bidirectional_range<decltype(iota_view)>);

    REQUIRE(*std::ranges::prev(std::          end(iota_view))  == +8);
    REQUIRE(*std::ranges::prev(std::ranges::  end(iota_view))  == +8);

    REQUIRE(*std::ranges::prev(
             std::ranges::next(std::        begin(iota_view))) == -7);
    REQUIRE(*std::ranges::prev(
             std::ranges::next(std::ranges::begin(iota_view))) == -7);

    for (auto index = 0; index != 10; ++index)
    {
        REQUIRE(in_range(iota_view                      , -7, +9, +1));
        REQUIRE(in_range(iota_view | std::views::reverse, +8, -8, -1));
    }
}


namespace
{
    template <std::integral integral>
    auto random_integer (const integral min_value,
                         const integral max_value) -> integral
    {
        assert(min_value <= max_value);

        auto probability_distribution = std::uniform_int_distribution
                                        {
                                            min_value, max_value
                                        };
        auto random_number_engine     = std::mt19937 { };

        return probability_distribution(random_number_engine);
    }
}


TEST_CASE("cxx::iota_view models a random_access_range", "[iota_view]")
{
    auto iota_view = cxx::iota_view { -7, +9 };

    static_assert(std::ranges::random_access_range<decltype(iota_view)>);

    for (auto index = 0; index != 100; ++index)
    {
        const auto random_index = random_integer(std::ptrdiff_t { 0 },
                                                 std::ssize(iota_view));

        const auto number = static_cast<int>(random_index) + iota_view.front();

        REQUIRE(iota_view[random_index] == number);
    }
}


TEST_CASE("cxx::iota_view models a view", "[iota_view]")
{
    // [ISO C++] - P2325R3: Views should not be required to be
    //                      default constructible
    // ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2325r3.html

    static_assert(std::ranges::view<cxx::iota_view<int>>);
}


TEST_CASE ("empty range", "[iota_view]")
{
    auto iota_view = cxx::iota_view { 0, 0 };

    for ([[maybe_unused]] auto number : iota_view)
    {
        FAIL();
    }

    REQUIRE(std::        begin(iota_view) == std::        end(iota_view));
    REQUIRE(std::ranges::begin(iota_view) == std::ranges::end(iota_view));

    REQUIRE(std::ranges:: size(iota_view) == std::ptrdiff_t { 0 });
    REQUIRE(std::ranges::ssize(iota_view) == std::ptrdiff_t { 0 });
}


TEST_CASE ("single element range", "[iota_view]")
{
    auto iota_view = cxx::iota_view { -7, -6 };

    REQUIRE(std::        begin(iota_view) != std::        end(iota_view));
    REQUIRE(std::ranges::begin(iota_view) != std::ranges::end(iota_view));

    REQUIRE(std::ranges:: size(iota_view) == std::ptrdiff_t { 1 });
    REQUIRE(std::ranges::ssize(iota_view) == std::ptrdiff_t { 1 });

    REQUIRE(in_range(iota_view, -7, -6));
}


TEST_CASE ("max size range", "[iota_view]")
{
    constexpr auto ptrdiff_max = std::numeric_limits<std::ptrdiff_t>::max();

    auto iota_view = cxx::iota_view { std::ptrdiff_t { 0 }, ptrdiff_max };

    REQUIRE(iota_view.front() ==                std::ptrdiff_t { 0 } );
    REQUIRE(iota_view. back() == (ptrdiff_max - std::ptrdiff_t { 1 }));

    REQUIRE(std::ranges:: size(iota_view) == ptrdiff_max);
    REQUIRE(std::ranges::ssize(iota_view) == ptrdiff_max);

    auto iter = std::ranges::begin(iota_view);
    iter += ptrdiff_max;
    REQUIRE(iter == std::ranges::end(iota_view));
    iter -= ptrdiff_max;
    REQUIRE(iter == std::ranges::begin(iota_view));
}


TEST_CASE ("range of all signed ints, except int_max", "[iota_view]")
{
    if constexpr (sizeof(std::ptrdiff_t) > sizeof(int))
    {
        constexpr auto int_min = std::numeric_limits<int>::min();
        constexpr auto int_max = std::numeric_limits<int>::max();

        auto iota_view = cxx::iota_view { int_min, int_max };

        REQUIRE(iota_view.front() ==  int_min     );
        REQUIRE(iota_view. back() == (int_max - 1));

        const auto size = std::abs(std::ptrdiff_t { int_min }) +
                                   std::ptrdiff_t { int_max };

        REQUIRE(std::ranges:: size(iota_view) == size);
        REQUIRE(std::ranges::ssize(iota_view) == size);
    }
}


TEST_CASE ("compare value-initialized iterators", "[iota_view]")
{
    // [ISO C++] - Working Draft, C++20 Standard
    //
    // ~ https://open-std.org/jtc1/sc22/wg21/docs/papers/2020/n4868.pdf#page=948
    //
    // 23.3.4.11 Concept forward_iterator
    //
    // (2) The domain of == for forward iterators
    //     is that of iterators over the same underlying sequence.
    //     However, value-initialized iterators of the same type
    //     may be compared and shall compare equal to
    //     other value-initialized iterators of the same type.
    //
    //     [Note 1: Value-initialized iterators behave as if
    //              they refer past the end of the same empty sequence.]

    // [C++ reference] Value initialization
    //
    // ~ https://en.cppreference.com/w/cpp/language/value_initialization

    typename cxx::iota_view<int>::iterator  left_iter { };
    typename cxx::iota_view<int>::iterator right_iter { };

    REQUIRE(  left_iter == right_iter );
    REQUIRE(!(left_iter != right_iter));
}
