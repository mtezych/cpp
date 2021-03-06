
#
# BSD 3-Clause License
#
# Copyright (c) 2018, mtezych
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

project(cl)

add_library               (cl STATIC include/cl/Platform.h
                                     include/cl/Device.h
                                     include/cl/Context.h
                                     include/cl/CommandQueue.h
                                     include/cl/Memory.h
                                     include/cl/Buffer.h
                                     include/cl/Image.h
                                     include/cl/Kernel.h
                                     include/cl/Program.h
                                     include/cl/Sampler.h
                                     include/cl/Event.h
                                         source/Platform.cpp
                                         source/Device.cpp
                                         source/Context.cpp
                                         source/CommandQueue.cpp
                                         source/Memory.cpp
                                         source/Buffer.cpp
                                         source/Image.cpp
                                         source/Kernel.cpp
                                         source/Program.cpp
                                         source/Sampler.cpp
                                         source/Event.cpp)

target_include_directories(cl PUBLIC include)

target_link_libraries     (cl OpenCL khronos platform gsl util)

set_target_properties     (cl PROPERTIES CXX_STANDARD          17
                                         CXX_STANDARD_REQUIRED ON)


add_executable            (cl-demo demo/cl-demo.cpp)

target_link_libraries     (cl-demo cl)

set_target_properties     (cl-demo PROPERTIES CXX_STANDARD          17
                                              CXX_STANDARD_REQUIRED ON)
