
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


#include <cxx/unique_ptr.hxx>

#include <catch2/catch.hpp>


namespace
{
    namespace check
    {
        template <typename value_type>
        auto empty (const cxx::unique_ptr<value_type>& unique_ptr) -> void
        {
            REQUIRE( unique_ptr.get() == nullptr);
            REQUIRE( unique_ptr       == nullptr);
            REQUIRE(!unique_ptr);
        }

        template <typename value_type>
        auto owns (const cxx::unique_ptr<value_type>& unique_ptr,
                   const                 value_type &      value) -> void
        {
            REQUIRE(unique_ptr.get() != nullptr);
            REQUIRE(unique_ptr       != nullptr);
            REQUIRE(unique_ptr);

            REQUIRE(*unique_ptr.get() == value);
            REQUIRE(*unique_ptr       == value);
        }
    }
}


TEST_CASE ("[unique_ptr] default constructor")
{
    const auto unique_ptr = cxx::unique_ptr<float> { };

    check::empty(unique_ptr);
}


TEST_CASE ("[unique_ptr] construct form nullptr")
{
    const auto unique_ptr = cxx::unique_ptr<char> { nullptr };

    check::empty(unique_ptr);
}


TEST_CASE ("[unique_ptr] construct form raw pointer")
{
    const auto unique_ptr = cxx::unique_ptr<char> { new char { '#' } };

    check::owns(unique_ptr, '#');
}


TEST_CASE ("[unique_ptr] destructor")
{
    struct object
    {
    private:
        bool& destructor_called;

    public:
        explicit constexpr object (bool& destructor_called) noexcept
        :
            destructor_called { destructor_called }
        { }

        constexpr ~object () noexcept
        {
            destructor_called = true;
        }
    };

    auto destructor_called = false;
    {
        const auto unique_ptr = cxx::unique_ptr<object>
                                {
                                    new object { destructor_called }
                                };
        REQUIRE(!destructor_called);
    }
    REQUIRE(destructor_called);
}


TEST_CASE ("[unique_ptr] move constructor")
{
    auto moved_from = cxx::unique_ptr<int> { new int { 8 } };
    check::owns(moved_from, 8);

    auto moved_to = std::move(moved_from);
    check::owns (moved_to, 8);
    check::empty(moved_from );
}


TEST_CASE ("[unique_ptr] move assignment")
{
    auto moved_from = cxx::unique_ptr<int> { new int { 7 } };
    auto moved_to   = cxx::unique_ptr<int> { nullptr       };
    check::owns (moved_from, 7);
    check::empty(moved_to     );

    moved_to = std::move(moved_from);
    check::owns (moved_to, 7);
    check::empty(moved_from );
}


TEST_CASE ("[unique_ptr] nullptr assignment")
{
    auto unique_ptr = cxx::unique_ptr<int> { new int { 9 } };
    check::owns(unique_ptr, 9);

    unique_ptr = nullptr;
    check::empty(unique_ptr);
}

TEST_CASE ("[unique_ptr] make_unique() function")
{
    class object
    {
    private:
        int   i;
        float f;

    public:
        constexpr explicit
        object (const int i = 0, const float f = 0.0f) noexcept
        :
            i { i },
            f { f }
        { }

        constexpr
        bool operator == (const object& other) const noexcept = default;
    };

    check::owns(cxx::make_unique<object>(8, 1.0f), object { 8, 1.0f });
    check::owns(cxx::make_unique<object>(8      ), object { 8, 0.0f });
    check::owns(cxx::make_unique<object>(       ), object { 0, 0.0f });
}


TEST_CASE ("[unique_ptr] arrow operator")
{
    struct object
    {
    private:
        mutable
        bool   const_method_called = false;
        bool mutable_method_called = false;

    public:
        auto const_method () const -> void
        {
            const_method_called = true;
        }

        auto mutable_method () -> void
        {
            mutable_method_called = true;
        }

        explicit operator bool () const
        {
            return const_method_called;
        }

        explicit operator bool ()
        {
            return const_method_called && mutable_method_called;
        }
    };

    const auto mutable_object = cxx::make_unique<object>();
    REQUIRE(!*mutable_object);

    mutable_object->  const_method();
    mutable_object->mutable_method();
    REQUIRE( *mutable_object);

    const auto const_object = cxx::make_unique<const object>();
    REQUIRE(!*const_object);

    const_object->const_method();
    REQUIRE( *const_object);
}


TEST_CASE ("[unique_ptr] bool conversion operator")
{
    auto owning_ptr = cxx::make_unique<int>(1);
    check::owns(owning_ptr, 1);

    if (owning_ptr) { SUCCEED(); } else { FAIL(); }

    auto empty_ptr = cxx::unique_ptr<int> { nullptr };
    check::empty(empty_ptr);

    if (!empty_ptr) { SUCCEED(); } else { FAIL(); }
}


TEST_CASE ("[unique_ptr] equivalence relation")
{
    auto owning_ptr = cxx::make_unique<int>(1);
    check::owns(owning_ptr, 1);

    auto empty_ptr  = cxx::unique_ptr<int> { nullptr };
    check::empty(empty_ptr);

    REQUIRE(owning_ptr !=    nullptr);  REQUIRE(nullptr    != owning_ptr);
    REQUIRE( empty_ptr ==    nullptr);  REQUIRE(nullptr    ==  empty_ptr);

    REQUIRE(owning_ptr == owning_ptr);  REQUIRE(owning_ptr !=  empty_ptr);
    REQUIRE( empty_ptr ==  empty_ptr);  REQUIRE( empty_ptr != owning_ptr);
}


TEST_CASE ("[unique_ptr] release")
{
    auto unique_ptr = cxx::make_unique<char>('#');
    check::owns(unique_ptr, '#');

    delete unique_ptr.release();
    check::empty(unique_ptr);
}


TEST_CASE ("[unique_ptr] reset to nullptr")
{
    auto unique_ptr = cxx::make_unique<bool>(true);
    check::owns(unique_ptr, true);

    unique_ptr.reset();
    check::empty(unique_ptr);
}


TEST_CASE ("[unique_ptr] reset to raw pointer")
{
    auto unique_ptr = cxx::make_unique<float>(0.1f);
    check::owns(unique_ptr, 0.1f);

    unique_ptr.reset(new float { 1.1f });
    check::owns(unique_ptr, 1.1f);
}
