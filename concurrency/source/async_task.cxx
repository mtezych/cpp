
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


#include <utility>

#include <thread>

#include <concepts>

#include <atomic>

#include <functional>

#include <cxx/channel.hxx>

#include <deque>

#include <mutex>

#include <condition_variable>

#include <optional>


namespace cxx
{
    // [C++ reference] - Reference declaration: Forwarding references
    //
    // ~ https://en.cppreference.com/w/cpp/language/reference
    //
    // Forwarding references are a special kind of references
    // that preserve the value category of a function argument,
    // making it possible to forward it by means of std::forward().
    //
    // Forwarding references are either:
    //
    // 1. function parameter of a function template declared as
    //    rvalue reference to cv-unqualified type template parameter (...):
    //
    //    template <typename type>
    //    auto fn (type&& ref) -> void { return; }
    //
    // 2. auto&&, except when deduced from a brace-enclosed initializer list
    //
    //    auto&& ref = func();


    // [WG21 N4164, N4262] - Forwarding References
    //
    // ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4164.pdf
    // ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4262.pdf


    // [Standard C++] - Universal References in C++11
    //
    // ~ https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers


    // [ISO C++] - Working Draft, C++20 Standard
    //
    // ~ http://open-std.org/jtc1/sc22/wg21/docs/papers/2020/n4868.pdf#page=446
    //
    // 13.10.3.2  Deducing template arguments from a function call
    //
    // A forwarding reference is
    // an rvalue reference to a cv-unqualified template parameter (...).
    //
    // If P is a forwarding reference and the argument is an lvalue,
    // the type "lvalue reference to A" is used in place of A for type deduction.


    // note: Forwarding references rely on the above hack in FTAD,
    //       which states that, when a function template accepts as a parameter
    //       an rvalue reference to a template parameter and
    //       that function template is invoked with an lvalue as an argument,
    //       then that template parameter will be deduced as an lvalue reference!
    //
    //       template <typename type>
    //       auto fn (type&& ref) -> void;
    //
    //       class object {    };          using const_object = const object;
    //
    //       auto variable = object { };
    //       const
    //       auto constant = object { };
    //
    //       fn(variable);         // calls fn<      object&>(      object& )
    //       fn(constant);         // calls fn<const object&>(const object& )
    //       fn(      object { }); // calls fn<      object >(      object&&)
    //       fn(const_object { }); // calls fn<const object >(const object&&)
    //
    //       After value category and mutability of the argument
    //       have slipped through the template parameter
    //       into the function parameter, reference collapsing takes place,
    //       resulting in perfectly forwarded argument.
    //
    //             object& &&  ->        object&
    //       const object& &&  ->  const object&
    //             object  &&  ->        object&&
    //       const object  &&  ->  const object&&


    // [Channel 9] Going Ntive 2013 - Interactive Panel: Ask Us Anything
    //
    // ~ https://channel9.msdn.com/Events/GoingNative/2013/Interactive-Panel-Ask-Us-Anything


    // note: The cxx::forward_as concept represents a deduced type,
    //       to which an rvalue reference will be appended,
    //       in order to perfectly forward a value of a concrete type.
    //
    //       auto perfectly_forward (cxx::forward_as<object> auto&& ref) -> void
    //       {
    //           func(std::forward<decltype(ref)>(ref));
    //       }
    //
    //       cxx::forward_as<object> auto&& ref = create_object();
    //
    template <typename deduced_type, typename concrete_type>
    concept forward_as = std::same_as<deduced_type,       concrete_type&> ||
                         std::same_as<deduced_type, const concrete_type&> ||
                         std::same_as<deduced_type,       concrete_type > ||
                         std::same_as<deduced_type, const concrete_type >;


    template <typename value_type>
    class concurrent_queue
    {
    private:
        std::deque<value_type>   values;
        std::mutex               mutex;
        std::condition_variable  ready;
        bool                     wait { true };

    public:
        auto interrupt () -> void
        {
            {
                auto lock = std::scoped_lock { mutex };

                wait = false;
            }
            ready.notify_all();
        }

        auto pop () -> std::optional<value_type>
        {
            auto lock = std::unique_lock { mutex };

            ready.wait(lock, [&] () noexcept -> bool
                             {
                                return !values.empty() || !wait;
                             });

            if (!values.empty())
            {
                auto value = std::move(values.front());

                values.pop_front();

                return value;
            }
            else
            {
                return std::nullopt;
            }
        }

        auto try_pop () -> std::optional<value_type>
        {
            auto lock = std::unique_lock { mutex, std::try_to_lock };

            if (lock.owns_lock() && !values.empty())
            {
                auto value = std::move(values.front());

                values.pop_front();

                return value;
            }
            else
            {
                return std::nullopt;
            }
        }

        auto push (cxx::forward_as<value_type> auto&& value) -> void
        {
            {
                auto lock = std::scoped_lock { mutex };

                values.emplace_back(std::forward<decltype(value)>(value));
            }
            ready.notify_one();
        }

        auto try_push (cxx::forward_as<value_type> auto&& value) -> bool
        {
            {
                auto lock = std::unique_lock { mutex, std::try_to_lock };

                if (!lock.owns_lock())
                {
                    return false;
                }
                values.emplace_back(std::forward<decltype(value)>(value));
            }
            ready.notify_one();
            return true;
        }
    };


    class executor
    {
    private:
        using task_type = std::move_only_function<auto () -> void>;

        cxx::concurrent_queue<task_type>     task_queue;
        std::thread                       worker_thread;

        static
        auto worker_main (cxx::concurrent_queue<task_type>& task_queue) -> void
        {
            while (true)
            {
                auto task = task_queue.pop();

                if (task != std::nullopt)
                {
                    std::invoke(*task);
                }
                else
                {
                    return;
                }
            }
        }

    public:
        executor ()
        :
            task_queue    {                                   },
            worker_thread { worker_main, std::ref(task_queue) }
        { }

        ~executor ()
        {
            task_queue.interrupt();

            worker_thread.join();
        }

        auto submit (std::invocable auto&& task) -> void
        {
            task_queue.push(task_type { std::forward<decltype(task)>(task) });
        }
    };


    class exec_token
    {
        cxx::channel::receiver completion;

        explicit exec_token (cxx::channel::receiver completion) noexcept
        :
            completion { std::move(completion) }
        { }

        friend auto async_exec (cxx::executor& executor,
                                std::invocable auto&& task) -> exec_token;

        friend auto sync_wait (const cxx::exec_token& exec_token) noexcept -> void;
    };


    auto async_exec (cxx::executor& executor,
                     std::invocable auto&& task) -> exec_token
    {
        auto [receiver, sender_generator] = cxx::channel::create(1);

        executor.submit([   task        = std::forward<decltype(task)>(task),
                            completion  = sender_generator() ] () mutable -> void
                        {
                            auto signal = cxx::channel::scoped_signal
                                          {
                                              std::move(completion)
                                          };

                            std::invoke(task);
                        });

        return cxx::exec_token { std::move(receiver) };
    }


    auto sync_wait (const cxx::exec_token& exec_token) noexcept -> void
    {
        cxx::channel::wait(exec_token.completion);
    }


    auto sync_exec (cxx::executor& executor,
                    std::invocable auto&& task) -> void
    {
        auto completion = std::atomic_flag { };

        executor.submit([&completion, &task]
                        {
                            std::invoke(task);

                            completion.test_and_set();
                            completion.notify_all();
                        });

        completion.wait(false);
    }
}


#include <cstdio>


auto main () -> int
{
    auto executor   = cxx::executor { };

    auto task       = [] () -> void
                      {
                          std::puts("Hello, from a task!");
                      };

    auto exec_token = cxx::async_exec(executor, task);

    cxx::sync_exec(executor, [] () -> void
                             {
                                 std::puts("Hello, from another task!");
                             });

    cxx::sync_wait(exec_token);

    return 0;
}
