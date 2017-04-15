
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

project(platform)

add_library               (platform STATIC include/platform/Library.h
                                           include/platform/Display.h
                                           include/platform/Window.h
                                                     source/Library.cpp)

target_include_directories(platform PUBLIC include)

target_link_libraries     (platform ${CMAKE_DL_LIBS})

if     (CMAKE_SYSTEM_NAME MATCHES Linux)

target_compile_definitions(platform PUBLIC PLATFORM_XLIB)
target_link_libraries     (platform platform-xlib)

elseif (CMAKE_SYSTEM_NAME MATCHES Android)

target_compile_definitions(platform PUBLIC PLATFORM_ANDROID)
target_link_libraries     (platform platform-android)

elseif (CMAKE_SYSTEM_NAME MATCHES Windows)

target_compile_definitions(platform PUBLIC PLATFORM_WINDOWS)
target_link_libraries     (platform platform-windows)

else ()

message("[platform] Only building for GNU/Linux, Android & Windows is supported.")

endif ()

set_target_properties     (platform PROPERTIES CXX_STANDARD          14
                                               CXX_STANDARD_REQUIRED ON)


add_executable            (platform-tests tests/platform-tests.cpp)

target_link_libraries     (platform-tests platform
                                          gtest)

set_target_properties     (platform-tests PROPERTIES CXX_STANDARD          14
                                                     CXX_STANDARD_REQUIRED ON)
