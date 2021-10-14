
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


#include <cxx/optional.hxx>

#include <catch2/catch.hpp>


namespace
{
    namespace check
    {
        template <typename value_type>
        auto empty (const cxx::optional<value_type>& optional) -> void
        {
            REQUIRE(!optional.has_value());
            REQUIRE(!optional);
        }

        template <typename value_type>
        auto owns (const cxx::optional<value_type>& optional,
                   const               value_type &    value) -> void
        {
            REQUIRE(optional.has_value());
            REQUIRE(optional);

            REQUIRE(optional.value() == value);
        }
    }

    namespace test
    {
        class object
        {
        private:
            int   i;
            float f;
            char  c;

        public:
            explicit
            constexpr object (int i = 0, float f = 0.0f, char c = '\0') noexcept
            :
                i { i },
                f { f },
                c { c }
            { }

            constexpr
            auto operator == (const object& other) const noexcept -> bool
            {
                return (this->i == other.i) &&
                       (this->f == other.f) &&
                       (this->c == other.c);
            }

            constexpr auto method () noexcept -> void
            { }
        };
    }
}


TEST_CASE ("[optional] default construct")
{
    const auto optional = cxx::optional<int> { };
    check::empty(optional);
}


TEST_CASE ("[optional] construct from nullopt")
{
    const auto optional = cxx::optional<int> { cxx::nullopt };
    check::empty(optional);
}


TEST_CASE ("[optional] emplace value")
{
    auto optional = cxx::optional<test::object> { };
    check::empty(optional);

    optional.emplace(7, 0.7f, '%');
    check::owns(optional, test::object { 7, 0.7f, '%' });
}


TEST_CASE ("[optional] assign nullopt")
{
    auto optional = cxx::optional<char> { };
    check::empty(optional);

    optional.emplace('*');
    check::owns(optional, '*');

    optional = cxx::nullopt;
    check::empty(optional);
}


TEST_CASE ("[optional] make_optional() function")
{
    const auto optional = cxx::make_optional<test::object>(11, 1.1f, '&');
    check::owns(optional, test::object { 11, 1.1f, '&' });
}


TEST_CASE ("[optional] idiomatic usage")
{
    auto optional = cxx::optional<test::object> { };
    check::empty(optional);

    optional.emplace(4, 0.8f, '#');
    check::owns(optional, test::object { 4, 0.8f, '#' });
    {
        if (optional) { SUCCEED(); } else { FAIL(); }

        optional->method();
    }
    optional.reset();
    check::empty(optional);
}

