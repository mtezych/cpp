
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Mateusz Zych
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef CXX_CONTRACTS
#define CXX_CONTRACTS


#include <cassert>


#if   defined(_MSC_VER)

    // [MSVC] - __assume
    // - https://docs.microsoft.com/en-us/cpp/intrinsics/assume
    #define cxx_assume(axiom) __assume(axiom)

#elif defined(__clang__)

    // [Clang] - Language Extensions
    // - https://clang.llvm.org/docs/LanguageExtensions.html#builtin-assume
    #define cxx_assume(axiom) __builtin_assume(axiom)

#elif defined(__GNUC__)

    // [GCC] - Other Built-in Functions
    // - https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
    #define cxx_assume(axiom) ((axiom) ? void() : __builtin_unreachable())

#else
    static_assert(false, "C++ compiler is not supported!");
#endif


#ifdef NDEBUG
    #define cxx_expects(axiom) cxx_assume(axiom)
#else
    #define cxx_expects(axiom)     assert(axiom)
#endif


#endif
