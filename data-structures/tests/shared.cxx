
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


#include <cxx/shared.hxx>

#include <catch2/catch.hpp>

#include <chrono>

#include <random>

#include <thread>


namespace
{
    namespace test
    {
        class object
        {
        private:

            char  c;
            float f;
            bool  b;

        public:

            explicit constexpr object (const  char c = '\0',
                                       const float f = 0.0f,
                                       const  bool b = false) noexcept
            :
                c { c }, f { f }, b { b }
            { }

            constexpr
            auto operator == (const object&) const noexcept -> bool = default;
        };
    }


    namespace check
    {
        template <std::equality_comparable type>
        auto share_identical_values (const cxx::shared<type>& left,
                                     const cxx::shared<type>& right) -> void
        {
            REQUIRE(  left ==   right); // shared handles are equal

            REQUIRE( *left ==  *right); // referenced values
            REQUIRE(&*left == &*right); // are not only equivalent,
                                        // but also identical
        }

        template <std::equality_comparable type>
        auto share_equivalent_values (const cxx::shared<type>& left,
                                      const cxx::shared<type>& right) -> void
        {
            REQUIRE(  left !=   right); // shared handles are not equal

            REQUIRE( *left ==  *right); // referenced values
            REQUIRE(&*left != &*right); // are equivalent,
                                        // but not identical
        }
    }
}


TEST_CASE ("[shared] bulk_create() single invocation, single shared handle")
{
    const auto [shared_handle] = cxx::shared<test::object>
                                    ::bulk_create<1>('#', 0.4f, true);

    REQUIRE(*shared_handle == test::object { '#', 0.4f, true });
}


TEST_CASE ("[shared] bulk_create() single invocation, multiple shared handles")
{
    const auto [ first_handle,
                second_handle ] = cxx::shared<test::object>
                                     ::bulk_create<2>('!', 0.1f, false);

    REQUIRE(* first_handle == test::object { '!', 0.1f, false });
    REQUIRE(*second_handle == test::object { '!', 0.1f, false });

    // note: A single call to cxx::shared<type>::bulk_create<count>()
    //       returns shared handles, which are all equal,
    //       because they all reference the exact same value.
    //
    //                                            +--------------------+
    //         +--------------------------------> |    test::object    |
    //         |              +-----------------> | { '!' 0.1f false } |
    //       +-|------------+-|-------------+     +--------------------+
    //       | first_handle | second_handle |
    //       +--------------+---------------+
    //
    check::share_identical_values(first_handle, second_handle);
}


TEST_CASE ("[shared] bulk_create() multiple invocations, single shared handle")
{
    const auto [  first_handle ] = cxx::shared<test::object>
                                      ::bulk_create<1>('?', 4.8f, true);

    const auto [ second_handle ] = cxx::shared<test::object>
                                      ::bulk_create<1>('?', 4.8f, true);

    REQUIRE(* first_handle == test::object { '?', 4.8f, true });
    REQUIRE(*second_handle == test::object { '?', 4.8f, true });

    // note: Distinct calls to cxx::shared<type>::bulk_create<count>()
    //       return shared handles, which are not equal,
    //       because they reference distinct values.
    //
    //                            +--------------------+
    //         +----------------> |    test::object    |
    //       +-|------------+     | { '?' 4.8f  true } |
    //       | first_handle |     +--------------------+
    //       +--------------+
    //                            +--------------------+
    //         +----------------> |    test::object    |
    //       +-|-------------+    | { '?' 4.8f  true } |
    //       | second_handle |    +--------------------+
    //       +---------------+
    //
    check::share_equivalent_values(first_handle, second_handle);
}


TEST_CASE ("[shared] bulk_create() multiple invocations, multiple shared handles")
{
    const auto [  first_handle,
                 second_handle ] = cxx::shared<test::object>
                                      ::bulk_create<2>('%', 0.5f, false);

    const auto [  third_handle,
                 fourth_handle ] = cxx::shared<test::object>
                                      ::bulk_create<2>('%', 0.5f, false);

    REQUIRE(* first_handle == test::object { '%', 0.5f, false });
    REQUIRE(*second_handle == test::object { '%', 0.5f, false });
    REQUIRE(* third_handle == test::object { '%', 0.5f, false });
    REQUIRE(*fourth_handle == test::object { '%', 0.5f, false });

    // note: Multiple invocations of cxx::shared<type>::bulk_create<count>()
    //       result in, creating arrays of shared handles, which are
    //       equal within a single array and non-equal across different arrays,
    //       due to referencing either the exact same value or distinct values.
    //
    //                                            +--------------------+
    //         +--------------------------------> |    test::object    |
    //         |              +-----------------> | { '%' 0.5f false } |
    //       +-|------------+-|-------------+     +--------------------+
    //       | first_handle | second_handle |
    //       +--------------+---------------+
    //                                            +--------------------+
    //         +--------------------------------> |    test::object    |
    //         |              +-----------------> | { '%' 0.5f false } |
    //       +-|------------+-|-------------+     +--------------------+
    //       | third_handle | fourth_handle |
    //       +--------------+---------------+
    //
    check::share_identical_values(first_handle, second_handle);
    check::share_identical_values(third_handle, fourth_handle);

    check::share_equivalent_values( first_handle,  third_handle);
    check::share_equivalent_values( first_handle, fourth_handle);
    check::share_equivalent_values(second_handle,  third_handle);
    check::share_equivalent_values(second_handle, fourth_handle);
}


namespace
{
    namespace test
    {
        class lifetime
        {
        private:
            bool& is_alive;

        public:
            explicit constexpr lifetime (bool& is_alive) noexcept
            :
                is_alive { is_alive }
            {
                is_alive = true;
            }

            constexpr ~lifetime () noexcept
            {
                is_alive = false;
            }
        };
    }
}


TEST_CASE ("[shared] destructor")
{
    auto is_alive = false;
    {
        [[maybe_unused]]
        auto [shared_handle] = cxx::shared<test::lifetime>
                                  ::bulk_create<1>(is_alive);

        REQUIRE(is_alive);
    }
    REQUIRE(!is_alive);
}


TEST_CASE ("[shared] move constructor")
{
    auto is_alive = false;

    auto [moved_from] = cxx::shared<test::lifetime>
                           ::bulk_create<1>(is_alive);

    REQUIRE(is_alive);
    {
        [[maybe_unused]]
        auto moved_to = std::move(moved_from);

        REQUIRE(is_alive);
    }
    REQUIRE(!is_alive);
}


TEST_CASE ("[shared] move assignment")
{
    auto is_first_alive = false;

    auto [moved_from] = cxx::shared<test::lifetime>
                           ::bulk_create<1>(is_first_alive);

    REQUIRE(is_first_alive);
    {
        auto is_second_alive = false;

        [[maybe_unused]]
        auto [moved_to] = cxx::shared<test::lifetime>
                             ::bulk_create<1>(is_second_alive);

        REQUIRE(is_first_alive); REQUIRE(is_second_alive);


        moved_to = std::move(moved_from);

        REQUIRE(is_first_alive); REQUIRE(!is_second_alive);
    }
    REQUIRE(!is_first_alive);
}


TEST_CASE ("[shared] reference counting, last handle destroys shared value")
{
    auto is_alive = false;

    auto shared_handles = cxx::shared<test::lifetime>
                             ::bulk_create<3>(is_alive);

    for (auto& shared_handle : shared_handles)
    {
        REQUIRE(is_alive); // note: each shared handle is a strong reference

        auto to_be_destroyed = std::move(shared_handle);
    }

    REQUIRE(!is_alive);
}


namespace
{
    namespace test
    {
        auto random_delay () -> void
        {
            auto rand_dev = std::random_device { };

            auto dist     = std::uniform_int_distribution<> { 1, 100 };

            auto duration = std::chrono::microseconds { dist(rand_dev) };

            std::this_thread::sleep_for(duration);
        }
    }
}


TEST_CASE ("[shared] reference counting is thread-safe")
{
    auto is_alive = false;

    auto shared_handles = cxx::shared<test::lifetime>
                             ::bulk_create<4>(is_alive);
    const
    auto thread_main = [&shared_handles] (std::size_t thread_index) -> void
    {
        test::random_delay();

        auto to_be_destroyed = std::move(shared_handles[thread_index]);
    };

    REQUIRE(is_alive);
    {
        const auto threads = std::array
        {
            std::jthread { thread_main, 0 },
            std::jthread { thread_main, 1 },
            std::jthread { thread_main, 2 },
            std::jthread { thread_main, 3 },
        };

        static_assert(std::size(shared_handles) == std::size(threads));
    }
    REQUIRE(!is_alive);
}


namespace
{
    namespace test
    {
        template <std::regular type>
        class value
        {
        private:
            type state;

        public:
            explicit constexpr value (const type& init_value)
            :
                state(init_value)
            { }

            [[nodiscard]]
            constexpr auto get ()       noexcept ->       type& { return state; }

            [[nodiscard]]
            constexpr auto get () const noexcept -> const type& { return state; }
        };
    }


    namespace check
    {
        template <typename test_value_type, std::regular type>
        requires
            std::same_as<test_value_type,       test::value<type>> ||
            std::same_as<test_value_type, const test::value<type>>
        //
        auto create_handle (const type& init_value) -> cxx::shared<test_value_type>
        {
            auto [value_handle] = cxx::shared<test_value_type>
                                     ::template bulk_create<1>(init_value);

            REQUIRE(  value_handle->get() == init_value);
            REQUIRE((*value_handle).get() == init_value);

            return std::move(value_handle);
        }


        template <std::regular type>
        auto assign_values (const cxx::shared<test::value<type>>& value_handle,
                            const type&  first_new_value,
                            const type& second_new_value) -> void
        {
            value_handle->get() = first_new_value;

            REQUIRE(  value_handle->get() == first_new_value);
            REQUIRE((*value_handle).get() == first_new_value);


            (*value_handle).get() = second_new_value;

            REQUIRE(  value_handle->get() == second_new_value);
            REQUIRE((*value_handle).get() == second_new_value);
        }


        template <typename test_value_type, std::regular type>
        requires
            std::same_as<test_value_type,       test::value<type>> ||
            std::same_as<test_value_type, const test::value<type>>
        //
        auto rebind_handle (cxx::shared<test_value_type>& value_handle,
                            const type& new_value) -> void
        {
            auto [new_handle] = cxx::shared<test_value_type>
                                    ::template bulk_create<1>(new_value);

            value_handle = std::move(new_handle);

            REQUIRE(  value_handle->get() == new_value);
            REQUIRE((*value_handle).get() == new_value);
        }
    }
}


TEST_CASE ("[shared] indirect access, mutable shared handle to mutable value")
{
    auto value_handle = check::create_handle<      test::value<char>>('#');

    check::assign_values(value_handle, '@', '$');
    check::rebind_handle(value_handle, '%');
}


TEST_CASE ("[shared] indirect access, const shared handle to mutable value")
{
    const
    auto value_handle = check::create_handle<      test::value<char>>('?');

    check::assign_values(value_handle, '!', '.');
}


TEST_CASE ("[shared] indirect access, mutable shared handle to const value")
{
    auto value_handle = check::create_handle<const test::value<char>>('&');

    check::rebind_handle(value_handle, '*');
}


TEST_CASE ("[shared] indirect access, const shared handle to const value")
{
    [[maybe_unused]]
    const
    auto value_handle = check::create_handle<const test::value<char>>('^');
}
