
#
# BSD 3-Clause License
#
# Copyright (c) 2017, mtezych
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

cmake_minimum_required (VERSION 3.20)

project (Concurrency VERSION 1.0.0 LANGUAGES CXX)


set (THREADS_PREFER_PTHREAD_FLAG ON)

find_package (Threads REQUIRED)


add_library                (concurrency INTERFACE)

target_compile_features    (concurrency INTERFACE cxx_std_20)

target_include_directories (concurrency INTERFACE include)


add_executable             (latch-demo)

target_compile_features    (latch-demo PRIVATE cxx_std_20)

target_sources             (latch-demo PRIVATE include/cxx/latch.hxx
                                                    source/latch_demo.cxx)

target_link_libraries      (latch-demo PRIVATE concurrency
                                               Threads::Threads)


add_executable             (async-task)

target_compile_features    (async-task PRIVATE cxx_std_23)

target_sources             (async-task PRIVATE source/async_task.cxx)

target_link_libraries      (async-task PRIVATE Threads::Threads
                                               data-structures)


add_executable             (sync-threads)

target_compile_features    (sync-threads PRIVATE cxx_std_20)

target_sources             (sync-threads PRIVATE source/sync_threads.cxx)

target_link_libraries      (sync-threads PRIVATE Threads::Threads)


add_executable             (producer-consumer)

target_compile_features    (producer-consumer PRIVATE cxx_std_17)

target_sources             (producer-consumer PRIVATE source/producer_consumer.cxx)

target_link_libraries      (producer-consumer PRIVATE Threads::Threads)
