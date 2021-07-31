
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


#include <cstddef>

#include <mutex>

#include <condition_variable>

#include <thread>

#include <iostream>


namespace concurrency
{
    enum sync_primitive
    {
        lock, atomic,
    };

    enum wait_mode
    {
        sleep, poll,
    };


    template <sync_primitive, wait_mode>
    class channel;

    template <>
    class channel <sync_primitive::lock, wait_mode::sleep>
    {
    private:
        char                    value { '\0' };
        std::mutex              mutex {      };
        std::condition_variable ready {      };

    public:
        auto send () -> void
        {
            {
                auto lock = std::scoped_lock { mutex };

                value = '#';
            }
            ready.notify_one();
        }

        auto receive () -> void
        {
            auto lock = std::unique_lock { mutex };

            ready.wait(lock, [this] () noexcept -> bool
                             {
                                 return value != '\0';
                             });

            std::cout << value;
        }
    };

    template <>
    class channel <sync_primitive::atomic, wait_mode::poll>
    {
    private:
        std::atomic<char> value { '\0' };

    public:
        auto send () -> void
        {
            value = '#';
        }

        auto receive () -> void
        {
            while (value == '\0')
            {
            }

            std::cout << value;
        }
    };

    template <>
    class channel <sync_primitive::atomic, wait_mode::sleep>
    {
    private:
        std::atomic<char> value { '\0' };

    public:
        auto send () -> void
        {
            value = '#';

            value.notify_one();
        }

        auto receive () -> void
        {
            value.wait('\0');

            std::cout << value;
        }
    };

    template <>
    class channel <sync_primitive::lock, wait_mode::poll>
    {
    private:
        char       value { '\0' };
        std::mutex mutex {      };

    public:
        auto send () -> void
        {
            auto lock = std::scoped_lock { mutex };

            value = '#';
        }

        auto receive () -> void
        {
            auto lock = std::unique_lock { mutex };

            while (value == '\0')
            {
                lock.unlock();

                // release the lock to enable forward progress of the sender

                lock.lock();
            }

            std::cout << value;
        }
    };


    template <sync_primitive sync_primitive, wait_mode wait_mode>
    auto sync_threads () -> void
    {
        auto channel = concurrency::channel<sync_primitive, wait_mode> { };

        auto thread  = std::jthread
                       {
                           [&] () { channel.send(); }
                       };

        channel.receive();
    }
}


auto main () -> int
{
    using concurrency::sync_primitive;
    using concurrency::wait_mode;

    concurrency::sync_threads<sync_primitive::  lock, wait_mode::sleep>();
    concurrency::sync_threads<sync_primitive::atomic, wait_mode:: poll>();
    concurrency::sync_threads<sync_primitive::atomic, wait_mode::sleep>();
    concurrency::sync_threads<sync_primitive::  lock, wait_mode:: poll>();

    return EXIT_SUCCESS;
}
