
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

project(cpp)

add_subdirectory(google-test)
add_subdirectory(google-benchmark)
add_subdirectory(gsl)
add_subdirectory(libsync)
add_subdirectory(platform-android)
add_subdirectory(utilities)
add_subdirectory(data-structures)
add_subdirectory(metaprogramming)
add_subdirectory(compile-time)
add_subdirectory(polymorphism)
add_subdirectory(allocator)
add_subdirectory(concurrency)
add_subdirectory(libdrm-example)
add_subdirectory(khronos)
add_subdirectory(cl)
add_subdirectory(egl)
add_subdirectory(gles)
add_subdirectory(platform-xlib)
add_subdirectory(platform-xcb)
add_subdirectory(vulkan)
add_subdirectory(platform-wayland)
add_subdirectory(util)
add_subdirectory(platform)
add_subdirectory(platform-windows)
add_subdirectory(ieee754)
add_subdirectory(concepts)
add_subdirectory(coroutines)
add_subdirectory(zlang)
add_subdirectory(gameboy)
