
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


# CMake Toolchain
cmake_minimum_required (VERSION 3.10)


# Target System
set(CMAKE_SYSTEM_NAME        Linux)
set(CMAKE_SYSTEM_VERSION       1.0)
set(CMAKE_SYSTEM_PROCESSOR  x86_64)


# Toolchain Root
set(CMAKE_FIND_ROOT_PATH "${TOOLCHAIN_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# Cross Compiler
set(CMAKE_C_COMPILER   "${TOOLCHAIN_ROOT}/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_ROOT}/llvm/bin/clang++")


# Cross Tools
set(CMAKE_AR           "${TOOLCHAIN_ROOT}/llvm/bin/llvm-ar"       CACHE FILEPATH "")
set(CMAKE_RANLIB       "${TOOLCHAIN_ROOT}/llvm/bin/llvm-ranlib"   CACHE FILEPATH "")
set(CMAKE_LINKER       "${TOOLCHAIN_ROOT}/llvm/bin/ld.lld"        CACHE FILEPATH "")
set(CMAKE_NM           "${TOOLCHAIN_ROOT}/llvm/bin/llvm-nm"       CACHE FILEPATH "")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_ROOT}/llvm/bin/llvm-objcopy"  CACHE FILEPATH "")
set(CMAKE_OBJDUMP      "${TOOLCHAIN_ROOT}/llvm/bin/llvm-objdump"  CACHE FILEPATH "")
set(CMAKE_STRIP        "${TOOLCHAIN_ROOT}/llvm/bin/llvm-strip"    CACHE FILEPATH "")


# Generator Tools
if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")

    set(CMAKE_MAKE_PROGRAM "${TOOLCHAIN_ROOT}/make/bin/make"      CACHE FILEPATH "")

elseif (CMAKE_GENERATOR STREQUAL "Ninja")

    set(CMAKE_MAKE_PROGRAM "${TOOLCHAIN_ROOT}/ninja/bin/ninja"    CACHE FILEPATH "")

else()
    message(FATAL_ERROR "Generator ${CMAKE_GENERATOR} is not supported!")
endif()
