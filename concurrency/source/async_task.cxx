
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

#include <cxx/concurrent_queue.hxx>


namespace cxx
{
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
