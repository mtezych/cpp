
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


#include <cxx/concepts.hxx>

#include <catch2/catch.hpp>


TEST_CASE ("integral types", "[losslessly_convertible_to]")
{
    static_assert( cxx::losslessly_convertible_to<  signed short,   signed  int>);
    static_assert( cxx::losslessly_convertible_to<unsigned   int, unsigned long>);
    static_assert(!cxx::losslessly_convertible_to<  signed short, unsigned long>);

    #if defined( INT16_MIN) && defined( INT32_MIN) && defined( INT64_MIN) && \
        defined( INT16_MAX) && defined( INT32_MAX) && defined( INT64_MAX) && \
        defined(UINT16_MAX) && defined(UINT32_MAX) && defined(UINT64_MAX)
    //
    // note: Fixed width integer types are optional.
    //
    static_assert( cxx::losslessly_convertible_to<std:: int32_t, std:: int64_t>);
    static_assert( cxx::losslessly_convertible_to<std:: int16_t, std:: int16_t>);
    static_assert(!cxx::losslessly_convertible_to<std:: int32_t, std:: int16_t>);

    static_assert( cxx::losslessly_convertible_to<std::uint16_t, std::uint32_t>);
    static_assert( cxx::losslessly_convertible_to<std::uint64_t, std::uint64_t>);
    static_assert(!cxx::losslessly_convertible_to<std::uint32_t, std::uint16_t>);

    static_assert( cxx::losslessly_convertible_to<std::uint32_t, std:: int64_t>);
    static_assert(!cxx::losslessly_convertible_to<std::uint32_t, std:: int16_t>);
    static_assert(!cxx::losslessly_convertible_to<std::uint64_t, std:: int64_t>);

    static_assert(!cxx::losslessly_convertible_to<std:: int16_t, std::uint64_t>);
    static_assert(!cxx::losslessly_convertible_to<std:: int64_t, std::uint16_t>);
    static_assert(!cxx::losslessly_convertible_to<std:: int32_t, std::uint32_t>);
    #endif
}


TEST_CASE ("non-integral types", "[losslessly_convertible_to]")
{
    // note: The type int is not convertible to void*,
    //       therefore is not losslessly convertible.
    //
    static_assert(!std::           convertible_to<int, void*>);
    static_assert(!cxx::losslessly_convertible_to<int, void*>);

    // note: Even though int* is convertible to void*,
    //       it has not been marked as losslessly convertible.
    //
    static_assert( std::           convertible_to<int*, void*>);
    static_assert(!cxx::losslessly_convertible_to<int*, void*>);

    static_assert( std::           convertible_to< float, double>);
    static_assert(!cxx::losslessly_convertible_to< float, double>);
    static_assert( std::           convertible_to<double,  float>);
    static_assert(!cxx::losslessly_convertible_to<double,  float>);

    enum color { red, green, blue };
    //
    static_assert( std::           convertible_to<color, int>);
    static_assert(!cxx::losslessly_convertible_to<color, int>);

    struct data { constexpr explicit(false) data (float) noexcept; };
    //
    static_assert( std::           convertible_to<float, data>);
    static_assert(!cxx::losslessly_convertible_to<float, data>);
}


namespace
{
    struct bar;
    struct foo { constexpr explicit(false) foo (bar) noexcept; };
    struct bar { constexpr explicit(false) bar (foo) noexcept; };
}

template <>
inline constexpr auto cxx::enable_losslessly_convertible_to<foo, bar> = true;

TEST_CASE ("user-defined types", "[losslessly_convertible_to]")
{
    static_assert(!cxx::losslessly_convertible_to<      bar ,       foo >);

    static_assert( cxx::losslessly_convertible_to<      foo ,       bar >);
    static_assert( cxx::losslessly_convertible_to<      foo , const bar >);
    static_assert( cxx::losslessly_convertible_to<      foo , const bar&>);

    static_assert( cxx::losslessly_convertible_to<const foo ,       bar >);
    static_assert( cxx::losslessly_convertible_to<const foo , const bar >);
    static_assert( cxx::losslessly_convertible_to<const foo , const bar&>);

    static_assert( cxx::losslessly_convertible_to<      foo&,       bar >);
    static_assert( cxx::losslessly_convertible_to<      foo&, const bar >);
    static_assert( cxx::losslessly_convertible_to<      foo&, const bar&>);

    static_assert( cxx::losslessly_convertible_to<const foo ,       bar >);
    static_assert( cxx::losslessly_convertible_to<const foo , const bar >);
    static_assert( cxx::losslessly_convertible_to<const foo , const bar&>);
}
