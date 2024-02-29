
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


#include <tuple>

#include <utility>

#include <type_traits>


namespace cxx
{
    namespace detail
    {
        template <typename tuple, typename callable, std::size_t... indices>
        constexpr
        auto for_each (tuple&& t, callable&& fn, std::index_sequence<indices...>)
        noexcept(noexcept(
            (std::forward<callable>(fn)(
             std::get<indices>(
             std::forward<tuple>(t))), ...))) -> void
        {
            (std::forward<callable>(fn)(
             std::get<indices>(
             std::forward<tuple>(t))), ...);
        }
    }

    template <typename tuple, typename callable>
    constexpr
    auto for_each (tuple&& t, callable&& fn)
    noexcept(noexcept(
        detail::for_each(std::forward<tuple>(t), std::forward<callable>(fn),
                         std::make_index_sequence<
                         std::tuple_size_v<
                         std::remove_reference_t<tuple>>> { }))) -> void
    {
        detail::for_each(std::forward<tuple>(t), std::forward<callable>(fn),
                         std::make_index_sequence<
                         std::tuple_size_v<
                         std::remove_reference_t<tuple>>> { });
    }
}


#include <cxx/executor.hxx>

#include <cxx/new_thread.hxx>

#include <cxx/thread_pool.hxx>

#include <catch2/catch.hpp>

#include <atomic>

#include <array>

#include <tuple>


namespace
{
    namespace test::execute_submitted_task_in_the_background
    {
        auto run (std::invocable auto create_executor) -> void
        //
        requires cxx::executor<std::invoke_result_t<decltype(create_executor)>>
        {
            auto flag = std::atomic_flag { false };

            std::invocable
            auto task = [&] () noexcept -> void
                        {
                            flag.test_and_set();
                            flag.notify_one();
                        };

            cxx::executor
            auto executor = std::invoke(create_executor);

            executor.submit(task);

            flag.wait(false);

            SUCCEED();
        }
    }


    namespace test::wait_for_completion_of_the_submitted_task
    {
        auto run (std::invocable auto create_executor) -> void
        //
        requires cxx::executor<std::invoke_result_t<decltype(create_executor)>>
        {
            auto result = '\0';

            std::invocable
            auto task = [&] () noexcept -> void
                        {
                            result = '#';
                        };
            {
                cxx::executor
                auto executor = std::invoke(create_executor);

                executor.submit(task);
            }
            REQUIRE(result == '#');
        }
    }


    namespace test::submit_invocable_tasks
    {
        auto run (std::invocable auto create_executor) -> void
        //
        requires cxx::executor<std::invoke_result_t<decltype(create_executor)>>
        {
            cxx::executor
            auto executor = std::invoke(create_executor);


            struct local
            {
                static constexpr auto function () noexcept -> void { }
            };

            auto& fn_ref =  local::function;
            auto* fn_ptr = &local::function;

            using fn_type = auto () noexcept -> void;

            static_assert(std::is_same_v<decltype(local::function), fn_type >);
            static_assert(std::is_same_v<decltype(         fn_ref), fn_type&>);
            static_assert(std::is_same_v<decltype(         fn_ptr), fn_type*>);

            executor.submit(local::function);
            executor.submit(         fn_ref);
            executor.submit(         fn_ptr);


            struct callable
            {
                constexpr auto operator () () const noexcept -> void { }
            };

            auto  fn_obj     = callable { };
            auto& fn_obj_ref = fn_obj;

            executor.submit(fn_obj    );
            executor.submit(fn_obj_ref);


            auto lambda     = [] () constexpr noexcept -> void { };
            auto lambda_ref = lambda;

            executor.submit(lambda    );
            executor.submit(lambda_ref);
        }
    }


    namespace test::submit_tasks_from_multiple_threads
    {
        auto run (std::invocable auto create_executor) -> void
        //
        requires cxx::executor<std::invoke_result_t<decltype(create_executor)>>
        {
            constexpr auto thread_count = 4;

            auto counter = std::atomic<int> { 0 };
            {
                cxx::executor
                auto executor = std::invoke(create_executor);

                auto thread_main = [&] () -> void
                {
                    auto task = [&counter] () -> void
                    {
                        counter.fetch_add(1, std::memory_order::relaxed);
                    };

                    executor.submit(task);
                };

                auto threads = std::array<std::jthread, thread_count>
                {
                    std::jthread { thread_main }, std::jthread { thread_main },
                    std::jthread { thread_main }, std::jthread { thread_main },
                };
            }
            REQUIRE(counter.load(std::memory_order::relaxed) == thread_count);
        }
    }


    constexpr auto   spawn_new_thread = [] () { return cxx:: new_thread { }; };

    constexpr auto create_thread_pool = [] () { return cxx::thread_pool { }; };


    constexpr auto tested_executors = std::tuple
                                      {
                                          &spawn_new_thread,
                                          &create_thread_pool,
                                      };


    TEST_CASE ("[executor] execute submitted task in the background")
    {
        cxx::for_each(tested_executors, [] (auto create_executor)
        {
            test::execute_submitted_task_in_the_background::run(create_executor);
        });
    }

    TEST_CASE ("[executor] wait for completion of the submitted task")
    {
        cxx::for_each(tested_executors, [] (auto create_executor)
        {
            test::wait_for_completion_of_the_submitted_task::run(create_executor);
        });
    }

    TEST_CASE ("[executor] submit invocable tasks")
    {
        cxx::for_each(tested_executors, [] (auto create_executor)
        {
            test::submit_invocable_tasks::run(create_executor);
        });
    }

    TEST_CASE ("[executor] submit tasks from multiple threads")
    {
        cxx::for_each(tested_executors, [] (auto create_executor)
        {
            test::submit_tasks_from_multiple_threads::run(create_executor);
        });
    }
}
