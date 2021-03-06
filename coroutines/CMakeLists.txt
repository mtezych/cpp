
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

cmake_minimum_required (VERSION 3.11)

project(coroutines VERSION 0.1.0 LANGUAGES CXX)


function(check_compiler LANG COMPILER_ID COMPILER_VERSION RESULT)

    set(${RESULT} FALSE PARENT_SCOPE)

    if(CMAKE_${LANG}_COMPILER_ID STREQUAL COMPILER_ID)

        if(CMAKE_${LANG}_COMPILER_VERSION VERSION_GREATER_EQUAL COMPILER_VERSION)

            set(${RESULT} TRUE PARENT_SCOPE)

        endif()

    endif()

endfunction()


#
# Clang 5.0.0 Release Notes
# ~ https://releases.llvm.org/5.0.0/tools/clang/docs/ReleaseNotes.html
#
# C++ Support in Clang
# ~ https://clang.llvm.org/cxx_status.html
#
check_compiler(CXX Clang 5.0.0 CLANG_SUPPORTS_COROUTINES)

#
# MSVC -> Enable coroutine support
# ~ https://docs.microsoft.com/en-us/cpp/build/reference/await-enable-coroutine-support
#
check_compiler(CXX MSVC 14.0 MSVC_SUPPORTS_COROUTINES)


if(CLANG_SUPPORTS_COROUTINES OR MSVC_SUPPORTS_COROUTINES)

    add_executable        (coroutines)
    target_sources        (coroutines PRIVATE coroutines.cpp)

  if(MSVC_SUPPORTS_COROUTINES)

    target_compile_options(coroutines PRIVATE /await)

  endif()

  if(CLANG_SUPPORTS_COROUTINES)

    target_compile_options(coroutines PRIVATE -stdlib=libc++ -fcoroutines-ts)
    target_link_libraries (coroutines PRIVATE -stdlib=libc++)

  endif()

    set_target_properties (coroutines PROPERTIES CXX_STANDARD          17
                                                 CXX_STANDARD_REQUIRED ON
                                                 CXX_EXTENSIONS        OFF)

else()

    message("[coroutines] At least Clang 5.0.0 or MSVC 14.0 is required.")

endif()
