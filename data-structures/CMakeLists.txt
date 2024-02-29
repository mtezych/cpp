
#
# BSD 3-Clause License
#
# Copyright (c) 2022, Mateusz Zych
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

cmake_minimum_required (VERSION 3.18)

project(data.structures VERSION 1.0.0 LANGUAGES CXX)


set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads)

find_package(Catch2 REQUIRED)


add_library                (data-structures INTERFACE)

target_compile_features    (data-structures INTERFACE cxx_std_20)

target_include_directories (data-structures INTERFACE include)

target_link_libraries      (data-structures INTERFACE utilities)


add_executable             (data-structures-tests)

target_compile_features    (data-structures-tests PRIVATE cxx_std_20)

target_sources             (data-structures-tests PRIVATE tests/catch2_main.cxx
                                                    include/cxx/allocator.hxx
                                                          tests/allocator.cxx
                                                    include/cxx/list.hxx
                                                          tests/list.cxx
                                                    include/cxx/vector.hxx
                                                          tests/vector.cxx
                                                    include/cxx/unique_ptr.hxx
                                                          tests/unique_ptr.cxx
                                                    include/cxx/indirect.hxx
                                                          tests/indirect.cxx
                                                    include/cxx/shared.hxx
                                                          tests/shared.cxx
                                                    include/cxx/tuple.hxx
                                                          tests/tuple.cxx
                                                    include/cxx/optional.hxx
                                                          tests/optional.cxx
                                                    include/cxx/span.hxx
                                                          tests/span.cxx
                                                    include/cxx/iota.hxx
                                                          tests/iota.cxx
                                                    include/cxx/chunk_evenly.hxx
                                                          tests/chunk_evenly.cxx
                                                    include/cxx/result.hxx
                                                          tests/result.cxx)

target_link_libraries      (data-structures-tests PRIVATE data-structures
                                                          Catch2::Catch2
                                                          Threads::Threads)
