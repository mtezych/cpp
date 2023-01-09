
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


#ifndef CXX_INTEGER_LIKE
#define CXX_INTEGER_LIKE


#include <type_traits>

#include <concepts>

#include <compare>

#include <limits>

#include <cstdint>


namespace cxx
{
    template <typename type>
    concept maybe_signed_integer_class = !std::is_arithmetic_v<type>
    //
    //
    // [ISO C++] - Working Draft, C++23 Standard
    //
    // 25.3.4.4 Concept weakly_incrementable             [iterator.concept.winc]
    //
    //  (2) A type I is an integer-class type if
    //      it is in a set of implementation-defined types
    //      that behave as integer types do, as defined below.
    //
    //      [ Note 1: An integer-class type is not necessarily a class type. ]
    //
    //  ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/n4917.pdf
    //
    //
    // note: Although integer-class types behave as integer types,
    //       they are a distinct set of types,
    //       and therefore do not model the std::integral concept.
    //
    //       This means that, for all integer-class types,
    //       the following meta-functions will always return false:
    //
    //        - std::is_integral<>
    //        - std::is_floating_point<>
    //        - std::is_arithmetic<>
    //        - std::is_signed<>
    //        - std::is_unsigned<>
    //
    //       Keep in mind, that an arithmetic type, that is,
    //       a type for which std::is_arithmetic<> meta-function returns true,
    //
    //       must model either std::integral or std::floating_point concept,
    //
    //       and both std::is_signed<> and std::is_unsigned<> meta-functions
    //       unconditionally return false for non-arithmetic types.
    //
    //
    // [C++ reference] -    Concepts library
    //                  (Core language concepts) : std::integral
    //                                             std::floating_point
    //
    //  ~ https://en.cppreference.com/w/cpp/concepts/integral
    //  ~ https://en.cppreference.com/w/cpp/concepts/floating_point
    //
    //
    // [C++ reference] - Metaprogramming library
    //                  (Primary type categories) : std::is_integral
    //                                              std::is_floating_point
    //
    //  ~ https://en.cppreference.com/w/cpp/types/is_integral
    //  ~ https://en.cppreference.com/w/cpp/types/is_floating_point
    //
    //
    // [C++ reference] -  Metaprogramming library
    //                  (Composite type categories) : std::is_arithmetic
    //
    //  ~ https://en.cppreference.com/w/cpp/types/is_arithmetic
    //
    //
    // [C++ reference] - Metaprogramming library
    //                      (Type properties)    : std::is_signed
    //                                             std::is_unsigned
    //
    //  ~ https://en.cppreference.com/w/cpp/types/is_signed
    //  ~ https://en.cppreference.com/w/cpp/types/is_unsigned
    //
    //
    // [C++ reference] -    Concepts library
    //                  (Core language concepts) : std::  signed_integral
    //                                             std::unsigned_integral
    //
    //  ~ https://en.cppreference.com/w/cpp/concepts/signed_integral
    //  ~ https://en.cppreference.com/w/cpp/concepts/unsigned_integral
    //
    //
    &&  std::numeric_limits<type>::is_specialized
    //
    //
    // [C++ reference] - Utilities library (Type support) : std::numeric_limits
    //
    //  If the implementation defines any integer-class types,
    //  specializations of std::numeric_limits<> must also be provided for them.
    //
    //  ~ https://en.cppreference.com/w/cpp/types/numeric_limits
    //
    //
    // [ISO C++] - Working Draft, C++23 Standard
    //
    // 17.3.5   Class template std::numeric_limits      [numeric.limits]
    //
    // 17.3.5.1 General                                 [numeric.limits.general]
    //
    //  (2) For all members declared static constexpr
    //      in the std::numeric_limits template,
    //
    //      specializations shall define these values in such a way
    //      that they are usable as constant expressions.
    //
    // 25.3.4.4 Concept weakly_incrementable             [iterator.concept.winc]
    //
    //  (5) For every integer-class type I,
    //      let B(I) be a unique hypothetical extended integer type
    //      of the same signedness with the same width as I.
    //
    //      [ Note 2: The corresponding
    //                hypothetical specialization std::numeric_limits<B(I)>
    //                meets the requirements on
    //                std::numeric_limits specializations for integral types. ]
    //
    //      For every integral type J, let B(J) be the same type as J.
    //
    // (11) For every (possibly cv-qualified) integer-class type I,
    //      std::numeric_limits<I> is specialized such that:
    //
    //       - each static data member m
    //         has the same value as std::numeric_limits<B(I)>::m, and
    //
    //       - each static member function f
    //         returns I(numeric_limits<B(I)>::f()).
    //
    //  ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/n4917.pdf
    //
    //
    &&  std::numeric_limits<type>::is_integer
    &&  std::numeric_limits<type>::is_signed
    && (std::numeric_limits<type>::digits
                                      >
                                     std::numeric_limits<std::intmax_t>::digits)
    //
    //
    // [ISO C++] - Working Draft, C++23 Standard
    //
    // 25.3.4.4 Concept weakly_incrementable             [iterator.concept.winc]
    //
    //  (3) The range of representable values of an integer-class type
    //      is the continuous set of values over which it is defined.
    //
    //      For any integer-class type,
    //      its range of representable values is either
    //
    //      −2^(N−1) to +2^(N−1)−1 (inclusive) for some integer N,
    //      in which case it is a signed-integer-class type, or
    //
    //      0 to 2^(N)−1 (inclusive) for some integer N,
    //      in which case it is an unsigned-integer-class type.
    //
    //      In both cases, N is called the width of the integer-class type.
    //
    //      The width of an integer-class type is greater than
    //      that of every integral type of the same signedness.
    //
    //  ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/n4917.pdf
    //
    //
    // [C++ reference] - Utilities library : std::numeric_limits<T>::digits
    //
    //  The value of std::numeric_limits<T>::digits
    //  is the number of digits in base-radix
    //  that can be represented by the type T without change.
    //
    //  For integer types, this is the number of bits
    //  not counting the sign bit and the padding bits (if any).
    //
    //  ~ https://en.cppreference.com/w/cpp/types/numeric_limits/digits
    //
    //
    // [ISO C] - working draft, C23 International Standard
    //
    // 7.22.1.5 Greatest-width integer types
    //
    // (1) The following type designates a signed integer type,
    //     other than a bit-precise integer type,
    //
    //     capable of representing any value of any signed integer type
    //     with the possible exceptions of
    //
    //     signed bit-precise integer types and of
    //
    //     signed extended integer types that are
    //     wider than long long and that are referred by the type definition
    //     for an exact width integer type:
    //
    //     intmax_t
    //
    //     The following type designates the unsigned integer type
    //     that corresponds to intmax_t:
    //
    //     uintmax_t
    //
    //     These types are required.
    //
    //  ~ https://open-std.org/JTC1/SC22/WG14/www/docs/n3054.pdf
    //
    //
    // [ThePhD] - A Special Kind of Hell : intmax_t in C and C++
    //
    //  ~ https://thephd.dev/intmax_t-hell-c++-c
    //
    //
    &&  std::constructible_from<  signed int, type>
    &&  std::constructible_from<unsigned int, type>
    &&  std::convertible_to    <  signed int, type>
    &&  std::convertible_to    <unsigned int, type>
    //
    //
    // [ISO C++] - Working Draft, C++23 Standard
    //
    // 25.3.4.4 Concept weakly_incrementable             [iterator.concept.winc]
    //
    //  (6) Expressions of integer-class type are
    //      explicitly convertible to any integer-like type, and
    //
    //      implicitly convertible to any integer-class type
    //      of equal or greater width and the same signedness.
    //
    //      Expressions of integral type are both
    //      implicitly and explicitly convertible to any integer-class type.
    //
    //      Conversions between integral and integer-class types and
    //      between two integer-class types do not exit via an exception.
    //
    //      The result of such a conversion is
    //      the unique value of the destination type that is congruent to
    //      the source modulo 2^N, where N is the width of the destination type.
    //
    //  (9) All integer-class types model std::regular (18.6) and
    //      std::three_way_comparable<std::strong_ordering> (17.12.4).
    //
    //  ~ https://open-std.org/JTC1/SC22/WG14/www/docs/n3054.pdf
    //
    //
    &&  std::regular             <type>
    &&  std::three_way_comparable<type, std::strong_ordering>;


    template <typename type>
    concept maybe_unsigned_integer_class = !std::is_arithmetic_v<type>
    //
    &&  std::numeric_limits<type>::is_specialized
    //
    &&  std::numeric_limits<type>::is_integer
    && !std::numeric_limits<type>::is_signed
    && (std::numeric_limits<type>::digits
                                     >
                                    std::numeric_limits<std::uintmax_t>::digits)
    //
    &&  std::constructible_from<  signed int, type>
    &&  std::constructible_from<unsigned int, type>
    &&  std::convertible_to    <  signed int, type>
    &&  std::convertible_to    <unsigned int, type>
    //
    &&  std::regular             <type>
    &&  std::three_way_comparable<type, std::strong_ordering>;


    template <typename type>
    concept maybe_integer_class = cxx::  maybe_signed_integer_class<type> ||
                                  cxx::maybe_unsigned_integer_class<type>;


    // [ISO C++] - Working Draft, C++23 Standard
    //
    // 25.3.4.4 Concept weakly_incrementable             [iterator.concept.winc]
    //
    //  (4) A type I other than cv bool is integer-like
    //      if it models std::integral<I> or if it is an integer-class type.
    //
    //      An integer-like type I is signed-integer-like
    //      if it models std::signed_integral<I> or
    //      if it is a signed-integer-class type.
    //
    //      An integer-like type I is unsigned-integer-like
    //      if it models std::unsigned_integral<I> or
    //      if it is an unsigned-integer-class type.
    //
    //  ~ https://open-std.org/JTC1/SC22/WG14/www/docs/n3054.pdf


    template <typename type>
    concept maybe_signed_integer_like =
    //
    std::signed_integral<type> || cxx::maybe_signed_integer_class<type>;


    template <typename type>
    concept maybe_unsigned_integer_like =
    //
    !std::same_as<std::remove_cv_t<type>, bool> &&
    //
    std::unsigned_integral<type> || cxx::maybe_unsigned_integer_class<type>;


    template <typename type>
    concept maybe_integer_like = cxx::  maybe_signed_integer_like<type> ||
                                 cxx::maybe_unsigned_integer_like<type>;
}


#endif
