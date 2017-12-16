
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

cmake_minimum_required (VERSION 3.1)

project(google-benchmark)

find_package(Threads REQUIRED)

add_library               (benchmark STATIC include/benchmark/benchmark.h
                                                          src/benchmark.cc
                                                          src/benchmark_api_internal.h
                                                          src/benchmark_register.cc
                                                          src/arraysize.h
                                                          src/check.h
                                                          src/colorprint.h
                                                          src/colorprint.cc
                                                          src/commandlineflags.h
                                                          src/commandlineflags.cc
                                                          src/complexity.h
                                                          src/complexity.cc
                                                          src/console_reporter.cc
                                                          src/counter.h
                                                          src/counter.cc
                                                          src/csv_reporter.cc
                                                          src/cycleclock.h
                                                          src/internal_macros.h
                                                          src/json_reporter.cc
                                                          src/log.h
                                                          src/mutex.h
                                                          src/re.h
                                                          src/reporter.cc
                                                          src/sleep.h
                                                          src/sleep.cc
                                                          src/statistics.h
                                                          src/statistics.cc
                                                          src/string_util.h
                                                          src/string_util.cc
                                                          src/sysinfo.cc
                                                          src/timers.h
                                                          src/timers.cc)

target_include_directories(benchmark PUBLIC include)

target_compile_definitions(benchmark PRIVATE HAVE_STD_REGEX
                                             HAVE_STEADY_CLOCK)

target_link_libraries     (benchmark ${CMAKE_THREAD_LIBS_INIT})

if(CMAKE_SYSTEM_NAME MATCHES Windows)

    target_link_libraries (benchmark Shlwapi)

endif()

set_target_properties     (benchmark PROPERTIES CXX_STANDARD          11
                                                CXX_STANDARD_REQUIRED ON)
