
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


#include <cxx/indirect.hxx>

#include <catch2/catch.hpp>


namespace
{
    namespace test
    {
        class object
        {
        private:
            int   i;
            float f;
            char  c;

        public:
            explicit constexpr object (const   int i = 0,
                                       const float f = 0.0f,
                                       const  char c = '\0') noexcept
            :
                i { i }, f { f }, c { c }
            { }

            constexpr
            auto operator == (const object&) const noexcept -> bool = default;

            [[nodiscard]]
            constexpr auto method () const noexcept -> bool
            {
                static_cast<void>(this);

                return false;
            }

            [[nodiscard]]
            constexpr auto method () noexcept -> bool
            {
                static_cast<void>(this);

                return true;
            }

            constexpr auto modify (const char character) noexcept -> void
            {
                --i;
                ++f;
                c = character;
            }
        };
    }
}


TEST_CASE ("[indirect] default constructor")
{
    auto indirect = cxx::indirect<test::object> { };

    REQUIRE(*indirect == test::object { });
}


TEST_CASE ("[indirect] in-place constructor")
{
    auto indirect = cxx::indirect<test::object> { 7, 0.1f, '~' };

    REQUIRE(*indirect == test::object { 7, 0.1f, '~' });
}


namespace
{
    namespace check
    {
        template <typename value_type>
        auto empty (const cxx::indirect<value_type>& indirect) -> void
        {
            // note: For instances of the cxx::indirect<> type,
            //       only moved from objects can be in the empty state.
            //
            //       Such an empty object must be either destroyed or
            //       a new value must be assigned to it.
            //
            REQUIRE(indirect.operator ->() == nullptr);
        }
    }
}


TEST_CASE ("[indirect] move constructor")
{
    auto moved_from = cxx::indirect<test::object> { 3, 0.6f, '@' };

    REQUIRE(*moved_from == test::object { 3, 0.6f, '@' });


    auto moved_to = std::move(moved_from);

    REQUIRE(*moved_to == test::object { 3, 0.6f, '@' });

    check::empty(moved_from);
}


TEST_CASE ("[indirect] move assignment")
{
    auto moved_from = cxx::indirect<test::object> { 3, 0.6f, '@' };
    auto moved_to   = cxx::indirect<test::object> { 7, 0.1f, '~' };

    REQUIRE(*moved_from == test::object { 3, 0.6f, '@' });
    REQUIRE(*moved_to   == test::object { 7, 0.1f, '~' });


    moved_to = std::move(moved_from);

    REQUIRE(*moved_to == test::object { 3, 0.6f, '@' });

    check::empty(moved_from);
}


TEST_CASE ("[indirect] copy constructor")
{
    auto original = cxx::indirect<test::object> { 3, 0.6f, '@' };

    REQUIRE(*original == test::object { 3, 0.6f, '@' });


    auto copy = original;

    REQUIRE(*copy     == test::object { 3, 0.6f, '@' });
    REQUIRE(*original == test::object { 3, 0.6f, '@' });

    REQUIRE(copy == original); // copies are equal


    copy->modify('!');

    REQUIRE(*copy     == test::object { 2, 1.6f, '!' });
    REQUIRE(*original == test::object { 3, 0.6f, '@' });

    REQUIRE(copy != original); // copies are disjoint
}


TEST_CASE ("[indirect] copy assignment")
{
    auto original = cxx::indirect<test::object> { 3, 0.6f, '@' };
    auto copy     = cxx::indirect<test::object> { 7, 0.1f, '~' };

    REQUIRE(*original == test::object { 3, 0.6f, '@' });
    REQUIRE(*copy     == test::object { 7, 0.1f, '~' });

    REQUIRE(copy != original);


    copy = original;

    REQUIRE(*copy     == test::object { 3, 0.6f, '@' });
    REQUIRE(*original == test::object { 3, 0.6f, '@' });

    REQUIRE(copy == original); // copies are equal


    copy->modify('?');

    REQUIRE(*copy     == test::object { 2, 1.6f, '?' });
    REQUIRE(*original == test::object { 3, 0.6f, '@' });

    REQUIRE(copy != original); // copies are disjoint
}


TEST_CASE ("[indirect] self assignment")
{
    auto  indirect  = cxx::indirect<test::object> { 1, 0.1f, '|' };
    auto& reference = indirect;

    REQUIRE(*indirect  == test::object { 1, 0.1f, '|' });
    REQUIRE(*reference == test::object { 1, 0.1f, '|' });


    indirect = reference; // must behave as-if called NOP (do-nothing operation)

    REQUIRE(*indirect  == test::object { 1, 0.1f, '|' });
    REQUIRE(*reference == test::object { 1, 0.1f, '|' });
}


TEST_CASE ("[indirect] arrow operator")
{
    const
    auto   const_indirect = cxx::indirect<test::object> { };
    auto mutable_indirect = cxx::indirect<test::object> { };

    REQUIRE(  const_indirect->method() == false);
    REQUIRE(mutable_indirect->method() ==  true);
}


namespace
{
    namespace check
    {
        auto dereference_indirect (auto&& indirect, auto&& answer) -> void
        {
            auto&& result = *std::forward<decltype(indirect)>(indirect);

            static_assert(std::is_same_v<decltype(result), decltype(answer)>);

            REQUIRE(result == answer);
        }
    }
}


TEST_CASE ("[indirect] dereference mutable indirect lvalue")
{
    auto indirect = cxx::indirect<test::object> { 8, 0.4f, '#' };

    auto answer   = test::object { 8, 0.4f, '#' };

    check::dereference_indirect(indirect, answer);
}


TEST_CASE ("[indirect] dereference const indirect lvalue")
{
    const auto indirect = cxx::indirect<test::object> { 8, 0.4f, '#' };

    const auto answer   = test::object { 8, 0.4f, '#' };

    check::dereference_indirect(indirect, answer);
}


TEST_CASE ("[indirect] dereference mutable indirect rvalue")
{
    check::dereference_indirect(cxx::indirect<test::object> { 8, 0.4f, '#' },
                                              test::object  { 8, 0.4f, '#' });
}


TEST_CASE ("[indirect] dereference const indirect rvalue")
{
    check::dereference_indirect(std::add_const_t<
                                cxx::indirect   <test::object>> { 8, 0.4f, '#' },
                                std::add_const_t<test::object>  { 8, 0.4f, '#' });
}


TEST_CASE ("[indirect] equality relation of identical objects")
{
    const auto   left = cxx::indirect<test::object> { 4, 0.8f, '?' };
    const auto& right = left;

    REQUIRE(  left ==   right);
    REQUIRE( *left ==  *right);
    REQUIRE(&*left == &*right);
}


TEST_CASE ("[indirect] equality relation of equivalent objects")
{
    const auto  left = cxx::indirect<test::object> { 4, 0.8f, '?' };
    const auto right = cxx::indirect<test::object> { 4, 0.8f, '?' };

    REQUIRE(  left ==   right);
    REQUIRE( *left ==  *right);
    REQUIRE(&*left != &*right);
}


TEST_CASE ("[indirect] inequality relation")
{
    const auto  left = cxx::indirect<test::object> { 7, 0.1f, '*' };
    const auto right = cxx::indirect<test::object> { 1, 0.7f, '^' };

    REQUIRE(  left !=   right);
    REQUIRE( *left !=  *right);
    REQUIRE(&*left != &*right);
}


namespace
{
    namespace check
    {
        constexpr auto compile_time_evaluation () noexcept -> bool
        {
            auto indirect = cxx::indirect<test::object> { 8, 4.2f, '%' };

            return *indirect == test::object { 8, 4.2f, '%' };
        };
    }
}


TEST_CASE ("[indirect] compile-time evaluation")
{
    static_assert(check::compile_time_evaluation());
}
