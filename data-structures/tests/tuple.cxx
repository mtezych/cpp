
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


#include <cxx/tuple.hxx>

#include <catch2/catch.hpp>


TEST_CASE ("[tuple] empty")
{
    constexpr auto tuple = cxx::tuple<> { };

    static_assert(cxx::tuple_size<decltype(tuple)> == 0);
}


TEST_CASE ("[tuple] singleton")
{
    constexpr auto tuple = cxx::tuple<int> { 7 };

    static_assert(cxx::tuple_size<decltype(tuple)> == 1);

    static_assert(std::is_same_v<cxx::tuple_element<0, decltype(tuple)>, int>);

    static_assert(cxx::get<0>(tuple) == 7);
}


TEST_CASE ("[tuple] pair")
{
    constexpr auto tuple = cxx::tuple<float, bool> { 0.1f, false };

    static_assert(cxx::tuple_size<decltype(tuple)> == 2);

    static_assert(std::is_same_v<cxx::tuple_element<0, decltype(tuple)>, float>);
    static_assert(std::is_same_v<cxx::tuple_element<1, decltype(tuple)>,  bool>);

    static_assert(cxx::get<0>(tuple) ==  0.1f);
    static_assert(cxx::get<1>(tuple) == false);
}


TEST_CASE ("[tuple] class template argument deduction")
{
    auto tuple = cxx::tuple { 0.1f, 8u, '%', true };

    using tuple_type = cxx::tuple<float, unsigned int, char, bool>;

    static_assert(std::is_same_v<decltype(tuple), tuple_type>);
}


TEST_CASE ("[tuple] get type of element at index")
{
    using tuple = cxx::tuple<int, float, char>;

    static_assert(std::is_same_v<cxx::tuple_element<0, tuple>,   int> &&
                  std::is_same_v<cxx::tuple_element<1, tuple>, float> &&
                  std::is_same_v<cxx::tuple_element<2, tuple>,  char>);

    static_assert(std::is_same_v<std::tuple_element_t<0, tuple>,   int> &&
                  std::is_same_v<std::tuple_element_t<1, tuple>, float> &&
                  std::is_same_v<std::tuple_element_t<2, tuple>,  char>);
}


TEST_CASE ("[tuple] get value of element at index")
{
    constexpr auto tuple = cxx::tuple { 7, 0.1f, '#' };

    static_assert(cxx::get<0>(tuple) ==    7);
    static_assert(cxx::get<1>(tuple) == 0.1f);
    static_assert(cxx::get<2>(tuple) ==  '#');
}


TEST_CASE ("[tuple] size")
{
    constexpr
    auto check_size = [] <typename ... types> (cxx::tuple<types...>) -> void
    {
        constexpr auto size = sizeof...(types);

        static_assert(                  cxx::tuple<types...>::size == size);
        static_assert(cxx::tuple_size  <cxx::tuple<types...>>      == size);
        static_assert(std::tuple_size_v<cxx::tuple<types...>>      == size);
    };

    check_size(cxx::tuple<                      > { });
    check_size(cxx::tuple<int                   > { });
    check_size(cxx::tuple<int, float            > { });
    check_size(cxx::tuple<int, float, char      > { });
    check_size(cxx::tuple<int, float, char, bool> { });
}


TEST_CASE ("[tuple] aggregate initialization")
{
    class immovable
    {
    private:
        int state;

    public:
        constexpr explicit immovable (const int param = 0) noexcept
        :
            state { param }
        { }

        constexpr ~immovable () noexcept = default;

        immovable (const immovable& ) = delete;
        immovable (      immovable&&) = delete;

        auto operator = (const immovable& ) -> immovable& = delete;
        auto operator = (      immovable&&) -> immovable& = delete;

        constexpr
        auto operator == (const immovable&) const noexcept -> bool = default;
    };

    constexpr auto tuple = cxx::tuple { immovable { 8 }, immovable { 4 } };

    static_assert((cxx::get<0>(tuple) == immovable { 8 }) &&
                  (cxx::get<1>(tuple) == immovable { 4 }));

    static_assert(std::is_aggregate_v<decltype(tuple)>);
}


TEST_CASE ("[tuple] structural type")
{
    constexpr auto tuple = cxx::tuple { 7, 1.7f, '^' };

    constexpr
    auto generic_lambda = [] <cxx::tuple<int, float, char> tuple> () -> void
    {
        static_assert(tuple == cxx::tuple { 7, 1.7f, '^' });
    };

    generic_lambda.template operator ()<tuple>();
}


TEST_CASE ("[tuple] structured binding")
{
    constexpr auto tuple = cxx::tuple { 8, 0.4f, '&' };

    const auto [i, f, c] = tuple;

    REQUIRE(i ==    8);
    REQUIRE(f == 0.4f);
    REQUIRE(c ==  '&');
}


namespace
{
    namespace check
    {
        auto tuple_get (auto&& tuple, auto&& answer) noexcept -> void
        {
            auto&& result = cxx::get<0>(std::forward<decltype(tuple)>(tuple));

            static_assert(std::is_same_v<decltype(result), decltype(answer)>);

            REQUIRE(result == answer);
        }
    }
}


TEST_CASE ("[tuple] get element from mutable lvalue tuple")
{
    auto tuple  = cxx::tuple { 7 };

    auto answer = 7;

    check::tuple_get(tuple, answer);
}


TEST_CASE ("[tuple] get element from const lvalue tuple")
{
    const auto tuple  = cxx::tuple { 7 };

    const auto answer = 7;

    check::tuple_get(tuple, answer);
}


TEST_CASE ("[tuple] get element from mutable rvalue tuple")
{
    check::tuple_get(cxx::tuple { 7 }, 7);
}


TEST_CASE ("[tuple] get element from const rvalue tuple")
{
    // [ISO C++] - Working Draft, C++11 Standard
    //
    // ~ http://open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf
    //
    // 3.10 [basic.lval] - Basic concepts: lvalues and rvalues
    //
    //  (4)  Class prvalues can have cv-qualified types;
    //       non-class prvalues always have cv-unqualified types.
    //
    const auto&& answer = 7;

    check::tuple_get(std::add_const_t<cxx::tuple<int>> { 7 },
                     std::forward<decltype(answer)>(answer));
}
