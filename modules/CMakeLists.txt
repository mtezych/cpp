
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

cmake_minimum_required (VERSION 3.12)

project(Modules VERSION 0.1.0 LANGUAGES CXX)


function(check_compiler LANG COMPILER_ID COMPILER_VERSION RESULT)

    set(${RESULT} FALSE PARENT_SCOPE)

    if(CMAKE_${LANG}_COMPILER_ID STREQUAL COMPILER_ID)

        if(CMAKE_${LANG}_COMPILER_VERSION VERSION_GREATER_EQUAL COMPILER_VERSION)

            set(${RESULT} TRUE PARENT_SCOPE)

        endif()

    endif()

endfunction()


#
# MSVC -> C++ Modules in Visual Studio 2015 Update 1
# ~ https://devblogs.microsoft.com/cppblog/c-modules-in-vs-2015-update-1
#
# MSVC -> Using C++ Modules in Visual Studio 2017
# ~ https://devblogs.microsoft.com/cppblog/cpp-modules-in-visual-studio-2017
#
check_compiler(CXX MSVC 14.0 MSVC_SUPPORTS_MODULES)


if(MSVC_SUPPORTS_MODULES)

    add_executable         (modules)

    target_sources         (modules PRIVATE modules.cpp)

    target_compile_features(modules PRIVATE cxx_std_20)

    target_compile_options (modules PRIVATE /experimental:module
                                            $<$<CONFIG:Debug>:/MDd>
                                            $<$<CONFIG:Release>:/MD>
                                            $<$<CONFIG:RelWithDebInfo>:/MD>
                                            $<$<CONFIG:MinSizeRel>:/MD>)

else ()

    message("[modules] MSVC 14.0 or later is required.")

endif()
