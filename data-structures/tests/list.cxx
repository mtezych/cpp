
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


#include <cxx/list.hxx>

#include <catch2/catch.hpp>


TEST_CASE ("[list] class template argument deduction")
{
    auto list = cxx::list { '$', '&', '#' };

    static_assert(std::is_same_v<decltype(list), cxx::list<char>>);
}


TEST_CASE ("[list] default constructor")
{
    auto list = cxx::list<char> { };

    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);

    REQUIRE(list. begin() == list. end());
    REQUIRE(list.cbegin() == list.cend());
}


TEST_CASE ("[list] std::initializer_list<> constructor")
{
    const auto init_list = std::initializer_list<char> { '%', '9' };

    const auto list      = cxx::list ( init_list );

    REQUIRE(!list.empty());

    REQUIRE(list.size () == std::size(init_list));
    REQUIRE(list.begin() != list.end());

    REQUIRE(list.front() == *std:: begin(init_list));
    REQUIRE(list.back () == *std::rbegin(init_list));
}

TEST_CASE ("[list] equivalence relation")
{
    const auto list = cxx::list { 1.1f, 0.1f };

    REQUIRE(list != cxx::list<float> {                        });
    REQUIRE(list != cxx::list<float> {       1.1f             });
    REQUIRE(list != cxx::list<float> {             0.1f       });
    REQUIRE(list == cxx::list<float> {       1.1f, 0.1f       });
    REQUIRE(list != cxx::list<float> {       1.1f, 0.1f, 0.1f });
    REQUIRE(list != cxx::list<float> { 1.1f, 1.1f, 0.1f       });
    REQUIRE(list != cxx::list<float> { 1.1f, 1.1f, 0.1f, 0.1f });
}


TEST_CASE ("[list] copy constructor")
{
    const auto list = cxx::list { '%', '9' };

    const auto copy = cxx::list { list };

    REQUIRE(list == copy);

    static_assert(std::is_same_v<decltype(list), decltype(copy)>);
}


TEST_CASE ("[list] copy assignment deallocating memory")
{
    const auto list = cxx::list { '!', '?' };

    auto copy = cxx::list { '&', '#', '@' };

    copy = list;

    REQUIRE(copy == cxx::list { '!', '?' });
}


TEST_CASE ("[list] copy assignment allocating memory")
{
    const auto list = cxx::list { '!', '?' };

    auto copy = cxx::list { '&' };

    copy = list;

    REQUIRE(copy == cxx::list { '!', '?' });
}


TEST_CASE ("[list] self copy assignment")
{
    auto list = cxx::list { '!', '?' };

    list = list;

    REQUIRE(list == cxx::list { '!', '?' });
}


TEST_CASE ("[list] range-based for loop")
{
    const
    auto init_list    = std::initializer_list<char> { '%', '9' };

    auto list         = cxx::list ( init_list );

    auto init_list_it = std::begin(init_list);

    for (auto& element : list)
    {
        REQUIRE(element == *init_list_it);

        ++init_list_it;
    }
}


TEST_CASE ("[list] iterator for loop")
{
    const
    auto init_list    = std::initializer_list<char> { '%', '9' };

    auto list         = cxx::list ( init_list );

    auto init_list_it = std::begin(init_list);

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        REQUIRE(*it == *init_list_it);

        ++init_list_it;
    }
}


TEST_CASE ("[list] const iterator for loop")
{
    const
    auto init_list    = std::initializer_list<char> { '$', '~', '@' };

    auto list         = cxx::list ( init_list );

    auto init_list_it = std::begin(init_list);

    for (auto it = list.cbegin(); it != list.cend(); ++it)
    {
        REQUIRE(*it == *init_list_it);

        ++init_list_it;
    }
}


TEST_CASE ("[list] front")
{
    const auto value = '%';

    const auto list  = cxx::list { value, '*', '&' };

    REQUIRE(list.front() == value);
}


TEST_CASE ("[list] back")
{
    const auto value = '%';

    const auto list  = cxx::list { '@', '&', '^', value };

    REQUIRE(list.back() == value);
}


TEST_CASE ("[list] push_back & pop_front")
{
    auto list = cxx::list { 7, 9 };

    list.push_back (4);    REQUIRE(list == cxx::list<int> {    7, 9, 4    });
    list.push_front(3);    REQUIRE(list == cxx::list<int> { 3, 7, 9, 4    });
    list.push_back (1);    REQUIRE(list == cxx::list<int> { 3, 7, 9, 4, 1 });
    list.pop_back  ( );    REQUIRE(list == cxx::list<int> { 3, 7, 9, 4    });
    list.pop_front ( );    REQUIRE(list == cxx::list<int> {    7, 9, 4    });
    list.pop_back  ( );    REQUIRE(list == cxx::list<int> {    7, 9       });
    list.pop_back  ( );    REQUIRE(list == cxx::list<int> {    7          });
    list.pop_front ( );    REQUIRE(list == cxx::list<int> {               });
    list.push_back (5);    REQUIRE(list == cxx::list<int> {       5       });
}
