
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2023, Mateusz Zych
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


#include <cxx/integer_like.hxx>

#include <catch2/catch.hpp>

#include <cstddef>


TEST_CASE ("sanity checks", "[is_custom_v]")
{
    // fundamental types are not custom types
    //
    static_assert(!cxx::is_custom_v< void                             >);
    static_assert(!cxx::is_custom_v< std::nullptr_t                   >);
    static_assert(!cxx::is_custom_v< int                              >);
    static_assert(!cxx::is_custom_v< float                            >);

    // compound types composed of only fundamental types are not custom types
    //
    static_assert(!cxx::is_custom_v< bool[8]                          >);
    static_assert(!cxx::is_custom_v< auto (float, int, char) -> void  >);
    static_assert(!cxx::is_custom_v< unsigned int*                    >);
    static_assert(!cxx::is_custom_v< float&                           >);
    static_assert(!cxx::is_custom_v< double&&                         >);
    static_assert(!cxx::is_custom_v< auto (*) (double*, short) -> int >);
    static_assert(!cxx::is_custom_v< auto (&) (bool, float&) -> int   >);

    enum enumeration {  };
    //
    static_assert(cxx::is_custom_v<enumeration>);

    union variant { };
    //
    static_assert(cxx::is_custom_v<variant>);

    struct structure { };
    //
    static_assert(cxx::is_custom_v<structure>);

    // arrays, functions, pointers and references are not custom types
    //
    static_assert(!cxx::is_custom_v< enumeration[4]                          >);
    static_assert(!cxx::is_custom_v< auto (enumeration, variant) -> int*     >);
    static_assert(!cxx::is_custom_v< variant*                                >);
    static_assert(!cxx::is_custom_v< enumeration&                            >);
    static_assert(!cxx::is_custom_v< structure&&                             >);
    static_assert(!cxx::is_custom_v< auto (*) (float) -> enumeration&        >);
    static_assert(!cxx::is_custom_v< auto (&) () -> variant                  >);
    static_assert(!cxx::is_custom_v< char (structure::*)                     >);
    static_assert(!cxx::is_custom_v< auto (variant::*) (float) -> structure& >);
}


#if defined(__GLIBCXX__) && defined(__STRICT_ANSI__) && defined(__SIZEOF_INT128__)
//
//
// [GNU libstdc++] - Manual: Macros
//
// ~ https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_macros.html
//
//
// [GCC] - Invoking GCC: Options Controlling C Dialect
//
// ~ https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html
//
//
// [GCC Bugzilla] - Bug 56540: No __SIZEOF__XXX__ macro for __float128
//
// ~ https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56540
//
//
TEST_CASE ("__int128", "[is_custom_v]")
{
    static_assert(std::is_object_v<    signed __int128>);
    static_assert(std::is_object_v<  unsigned __int128>);

    static_assert(std::is_compound_v<  signed __int128>);
    static_assert(std::is_compound_v<unsigned __int128>);

    // note: When GCC or Clang is compiling C++ code in a mode,
    //       which provides strict conformance to the ISO standard,
    //
    //       the GNU's implementation of the C++ standard library (libstdc++)
    //       considers __int128 a compound type and an object type.

    static_assert(cxx::is_custom_v<           signed __int128>);
    static_assert(cxx::is_custom_v<         unsigned __int128>);

    static_assert(cxx::is_custom_v<   const   signed __int128>);
    static_assert(cxx::is_custom_v<   const unsigned __int128>);

    static_assert(cxx::is_custom_v<volatile   signed __int128>);
    static_assert(cxx::is_custom_v<volatile unsigned __int128>);
}
#endif
