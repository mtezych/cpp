
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


#include <thread>

#include <queue>

#include <mutex>

#include <condition_variable>

#include <chrono>

#include <iostream>


namespace concurrency
{
    class product
    {
    private:
        char material;

    public:
        explicit product (const char material = '\0')
        :
            material { material }
        {
            std::this_thread::sleep_for(std::chrono::seconds { 1 });
        }

        friend auto operator << ( std::ostream& ostream,
                                 const product& product) -> std::ostream&
        {
            return ostream << product.material;
        }
    };


    class market
    {
    private:
        std::queue<product>     products;
        std::mutex              mutex;
        std::condition_variable cond_var;

    public:
        auto produce () -> void
        {
            auto produced_product = product { '#' };
            {
                auto lock = std::scoped_lock { mutex };

                products.push(produced_product);
            }
            cond_var.notify_one();
        }

        auto consume () -> void
        {
            auto consumed_product = [this]
            {
                auto lock = std::unique_lock { mutex };

                cond_var.wait(lock, [this] () noexcept -> bool
                                    {
                                        return !products.empty();
                                    });

                auto consumed_product = products.front();

                products.pop();

                return consumed_product;
            }();

            std::cout << consumed_product << std::endl;
        }
    };
}


auto main () -> int
{
    constexpr auto supply_and_demand = 8;

    auto market   = concurrency::market { };

    auto producer = std::thread
    {
        [&market] ()
        {
            for (auto iter = 0; iter != supply_and_demand; ++iter)
            {
                market.produce();
            }
        }
    };

    auto consumer = std::thread
    {
        [&market] ()
        {
            for (auto iter = 0; iter != supply_and_demand; ++iter)
            {
                market.consume();
            }
        }
    };

    producer.join();
    consumer.join();

    return 0;
}
