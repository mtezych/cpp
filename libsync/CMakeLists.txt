
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

project(libsync)

if(CMAKE_SYSTEM_NAME MATCHES Android)

 add_library               (sync SHARED include/sync/sync.h
                                        sw_sync.h
                                        sync.c)
 target_include_directories(sync PUBLIC include)

 add_executable            (sync_test sync_test.c)
 target_include_directories(sync_test PUBLIC include)
 target_link_libraries     (sync_test sync)

 add_executable            (sync-unit-tests tests/sync_test.cpp)
 target_include_directories(sync-unit-tests PUBLIC include .)
 target_link_libraries     (sync-unit-tests sync gtest)
 set_target_properties     (sync-unit-tests PROPERTIES CXX_STANDARD          11
                                                       CXX_STANDARD_REQUIRED ON)

 # Android always bundles pthread library with glibc and bionic.
 # http://mobilepearls.com/labs/native-android-api/#pthreads

else()

 message("[libsync] Only building for Android is supported.")

endif()
