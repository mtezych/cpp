
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


#ifndef CXX_THREAD_POOL
#define CXX_THREAD_POOL


#include <cxx/executor.hxx>

#include <cxx/concurrent_queue.hxx>

#include <concepts>

#include <cstddef>

#include <utility>

#include <atomic>

#include <span>

#include <vector>

#include <functional>

#include <thread>


namespace cxx
{
    class thread_pool
    {
    private:
        using task_type  = std::move_only_function<auto () -> void>;
        using task_queue = cxx::  concurrent_queue<   task_type   >;

        std::vector<std::thread>    worker_threads;
        std::vector<task_queue>        task_queues;
        std::atomic<unsigned int>   submit_counter;
        //
        // note: In order to support submitting tasks indefinitely,
        //       overflow of the submit_counter must be well-defined.

        static
        auto worker_main (const std::ptrdiff_t         worker_index,
                          const std::span<task_queue>   task_queues) -> void
        {
            const auto execute_task = [&] () -> bool
            {
                for (auto n = std::size_t { 0 }; n != task_queues.size(); ++n)
                {
                    auto queue_index = (worker_index + n) % task_queues.size();

                    auto task = task_queues[queue_index].try_pop();

                    if (task != std::nullopt)
                    {
                        std::invoke(*task); return true;
                    }
                }

                auto task = task_queues[worker_index].pop();

                if (task != std::nullopt)
                {
                    std::invoke(*task); return true;
                }
                else
                {
                    return false; // no more tasks to execute
                }
            };

            while (execute_task()) { }
        }

    public:
        explicit
        thread_pool (std::ptrdiff_t size = std::thread::hardware_concurrency())
        :
            worker_threads {                                },
            task_queues    { static_cast<std::size_t>(size) },
            submit_counter {                             0u }
        {
            worker_threads.reserve(size);

            auto worker_index = std::ptrdiff_t { 0 };

            for (; worker_index != size; ++worker_index)
            {
                worker_threads.emplace_back(worker_main, worker_index,
                                            std::span { task_queues });
            }
        }

        ~thread_pool ()
        {
            for (auto& task_queue : task_queues)
            {
                task_queue.interrupt();
            }

            for (auto& worker_thread : worker_threads)
            {
                worker_thread.join();
            }
        }

        auto submit (std::invocable auto&& task) -> void
        {
            auto task_value = task_type { std::forward<decltype(task)>(task) };

            const auto submit_index = // round-robin scheduling
                       submit_counter.fetch_add(1, std::memory_order::relaxed) + 1;

            for (auto n = std::size_t { 0 }; n != task_queues.size(); ++n)
            {
                const auto queue_index = (submit_index + n) % task_queues.size();

                if (task_queues[queue_index].try_push(std::move(task_value)))
                {
                    return;
                }
            }

            const auto queue_index = submit_index % task_queues.size();

            task_queues[queue_index].push(std::move(task_value));
        }
    };

    static_assert(cxx::executor<cxx::thread_pool>);
}


#endif
