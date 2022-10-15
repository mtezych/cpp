
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


#ifndef CXX_IOTA
#define CXX_IOTA


#include <cxx/detect_overflow.hxx>

#include <cstddef>

#include <type_traits>

#include <concepts>

#include <iterator>

#include <ranges>

#include <limits>

#include <utility>

#include <cassert>


namespace cxx
{
    template <std::       integral  element_type,
              std::signed_integral iterdiff_type = std::ptrdiff_t>
    //
    // note: Even though, theoretically, the maximum size of an object,
    //       that is, the number of bytes
    //       in the representation of the largest possible object,
    //
    //       is limited only to the maximum std::size_t value (size_max),
    //       if only due to the fact that
    //       the sizeof() operator yields an expression of type std::size_t,
    //
    //       in practice, C++ implementations often
    //       additionally limit the size of the largest possible object
    //       to the maximum std::ptrdiff_t value (ptrdiff_max).
    //
    //
    // [ISO C++] - Working Draft, C++20 Standard
    //
    //  6.8.3 Basics: Types - Compound types
    //
    //    (2) Constructing a type such that
    //        the number of bytes in its object representation
    //        exceeds the maximum value representable in the type std::size_t
    //        is ill-formed.
    //
    // ~ https://open-std.org/jtc1/sc22/wg21/docs/papers/2020/n4868.pdf#page=85
    //
    //
    // [C++ reference] - Utilities library: Type support - std::size_t
    //
    //  Notes
    //    The std::size_t can store
    //    the maximum size of a theoretically possible object of any type,
    //    including an array.
    //
    //    A type whose size cannot be represented by std::size_t is ill-formed.
    //
    // ~ https://en.cppreference.com/w/cpp/types/size_t
    //
    //
    //       To understand why C++ implementations willingly prohibit
    //       defining objects, which size exceed the ptrdiff_max limit,
    //       consider a C-style array of more than ptrdiff_max elements:
    //
    //           static_assert(sizeof(std::size_t) >= sizeof(std::ptrdiff_t));
    //           constexpr auto size = std::size_t { ptrdiff_max } + 1;
    //           element array [size];
    //
    //       Such an array does not meet the semantic requirements of
    //       the std::ranges::random_access_range, because:
    //
    //        1. It is not possible to advance the begin() pointer
    //           by the distance equal to the size() of the array,
    //           in order obtain the end() pointer,
    //
    //           since the size() of such an array is
    //           simply not representable as a std::ptrdiff_t value.
    //
    //               auto* array_end = std::begin(array);
    //               array_end += std::numeric_limits<std::ptrdiff_t>::max();
    //               array_end += 1;
    //
    //           Essentially multiple advance operations are needed,
    //           in order to reach the end of such an array from its beginning,
    //           breaking the assumption that arrays provide random access.
    //
    //        2. Computing the distance between
    //           the end() pointer and the begin() pointer,
    //           in order to obtain the size of such an array,
    //           invokes undefined behaviour (UB),
    //
    //           because, again, the size() of the array is
    //           not representable as a std::ptrdiff_t value.
    //
    //           auto array_size = array_end - std::begin(array); // overflow
    //
    //
    // [C++ reference] - Utilities library: Type support - std::ptrdiff_t
    //
    //  Notes
    //    If an array is so large
    //    (greater than ptrdiff_max elements, but less than size_max bytes),
    //
    //    that the difference between two pointers
    //    may not be representable as std::ptrdiff_t,
    //
    //    the result of subtracting two such pointers is undefined.
    //
    // ~ https://en.cppreference.com/w/cpp/types/ptrdiff_t
    //
    //
    //       One might argue, that the above issues will never be encountered,
    //       as long as sizeof(element) > 1.
    //
    //       Yes, this is indeed true. However, the C++ standard permits
    //       accessing the representation of any object,
    //       that is, its underlying storage, in the form of an array of bytes:
    //
    //           auto object       = ...;
    //           auto storage_data = static_cast<std::byte*>(
    //                               static_cast<     void*>(&object));
    //           auto storage_size = sizeof(object);
    //
    //       After all, the C++ abstract machine guarantees that
    //       memory can be accessed with the granularity of individual bytes,
    //       as long as objects are sufficiently aligned in memory.
    //
    //       Therefore,
    //       even an array, containing no more than ptrdiff_max elements,
    //       can be affected by the issues described above,
    //       as long as, its size in bytes exceeds the ptrdiff_max limit and
    //       at some point in time its underlying storage is accessed.
    //
    //       In some sense, this is a relatively rare operation,
    //       since obtaining object's representation is usually performed
    //       in order to serialize an object and
    //       store its raw bytes in a file or send them via network.
    //
    //
    //       However, essentially any operation,
    //       which ignores the structure of an object
    //       and treats it as a chunk of raw memory / an array of raw bytes,
    //       can suffer from the issues described above.
    //
    //       Consider the fact that, even though,
    //       abstractly, pointer arithmetic is performed element-wise,
    //
    //       C++ implementations, which implement pointers
    //       as memory addresses, that is, as unsigned integers
    //       representing indexes of particular bytes in memory,
    //
    //       must perform pointer arithmetic byte-wise,
    //       taking into account the size of the pointed-to type.
    //
    //           int array_of_ints [8] = { };
    //           auto* ptr = std::begin(array_of_ints);
    //           ++ptr; // internally increments memory address by sizeof(int)
    //
    //       Additionally, such C++ implementations often define std::uintptr_t,
    //       such that it has the exact same object and value representation
    //       as pointers to objects and is synonymous with the std::size_t type.
    //
    //           static_assert(sizeof(std::uintptr_t) == sizeof(element*));
    //           static_assert(std::is_same_v<std::size_t, std::uintptr_t>);
    //
    //       Moreover, to ensure that
    //       pointer arithmetic does not incur any unnecessary overhead,
    //       it is common for such C++ implementations to define std::intptr_t
    //       to have the exact same size as pointers to objects
    //       and to be synonymous with the std::ptrdiff_t type.
    //
    //           static_assert(sizeof(std::intptr_t) == sizeof(element*));
    //           static_assert(std::is_same_v<std::ptrdiff_t, std::intptr_t>);
    //
    //       Consequently, computing the size of an array,
    //       by subtracting the begin() pointer from the end() pointer,
    //
    //       can overflow on such C++ implementations, even in the case when
    //       the array contains no more than ptrdiff_max elements, as long as,
    //
    //       the size in bytes of that array exceeds the ptrdiff_max limit.
    //
    //           static_assert(sizeof(element) > 1);
    //           element array [ptrdiff_max];
    //           auto array_size = std::end(array) - std::begin(array);
    //
    //       Essentially, on such C++ implementations,
    //       there are two methods of computing the distance between pointers:
    //
    //        1. subtract memory addresses, that is std::uintptr_t values,
    //           then interpret the result as a std::ptrdiff_t value,
    //           and lastly divide the converted value by the sizeof(element)
    //
    //        2. divide each memory address by the sizeof(element),
    //           then subtract memory addresses,
    //           and lastly interpret the result as a std::ptrdiff_t value
    //
    //       Of course, the first method is more efficient,
    //       and therefore commonly used in practice.
    //
    //       However, that method also requires
    //       the result of subtracting memory addresses
    //       to be representable as a std::ptrdiff_t value.
    //
    //       This effectively forces such C++ implementations,
    //       to prohibit the creation of objects,
    //       which size in bytes exceeds the ptrdiff_max limit.
    //
    //
    //       Understanding that it is incredibly rare for a C-style array
    //       to contain more than ptrdiff_max elements, and even
    //       the size of a C++ object is limited to the ptrdiff_max bytes,
    //
    //       is it quite clear, that the std::ptrdiff_t type is
    //       the preferred signed integral type used for
    //       counting objects stored physically in memory.
    //
    //       Moreover, since the C++ standard defines pointer arithmetic
    //       using the std::ptrdiff_t type, it is almost guaranteed that
    //       hardware can process std::ptrdiff_t values incredibly efficiently.
    //
    //       In contrast, the std::ranges::iota_view defines
    //       the difference_type of its iterator, as a signed integral type
    //       or as an implementation-provided, integer-like type, which is often
    //       guaranteed to never overflow, due to sufficiently large width.
    //
    //
    // [C++ reference] Ranges library: std::ranges::iota_view<>::iterator
    //
    // ~ https://en.cppreference.com/w/cpp/ranges/iota_view/iterator
    //
    //
    // [ISO C++] P1522R1: Iterator Difference Type and Integer Overflow
    //
    // ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1522r1.pdf
    //
    //
    //       Of course, such a guarantee imposes additional overhead,
    //       since it requires using a signed integral type, which
    //       width is greater than the size of the weakly_incrementable type.
    //
    //
    // [GCC Git] - (GNU) GCC libstdc++: std::ranges::iota_view
    //
    // ~ https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/include/std/ranges;hb=refs/heads/trunk#l300
    //
    //
    //       To satisfy the described guarantee, in extreme cases,
    //       such as, when the initial value is a 128-bit integer,
    //       the iterator of the std::ranges::iota_view
    //       must define the difference_type as an integer-like class
    //       emulating in software 129-bit integer arithmetic.
    //
    //
    // [GCC Git] - (GNU) GCC libstdc++: std::ranges::__detail::__max_diff_type
    //
    // ~ https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/include/bits/max_size_type.h;hb=refs/heads/trunk#l440
    //
    class iota_view
    :
        public
        std::ranges::view_interface<iota_view<element_type, iterdiff_type>>
    {
    public:
        class iterator
        {
        private:
            element_type value;

        public:
            // note: In C++20, the model of forward_iterators has been relaxed,
            //       permitting the indirection (dereference) operator
            //       to return by value / return a prvalue.
            //
            //       However, in C++98 it can be assumed that
            //       the indirection operator of a forward_iterator
            //       returns an lvalue reference.
            //
            //       This requires cxx::iota_view::iterator to advertise itself
            //       as a C++20 random_access_iterator and C++98 input_iterator.
            //
            //
            // [stack overflow] - What is the difference between
            //                    iterator_category and iterator_concept
            //                    in C++20?
            //
            //   There are differences between the C++17 (C++98) iterator model
            //   and the C++20 ranges iterator model
            //   that are not backwards compatible. The two big ones are:
            //
            //   1. The C++98 model requires that forward iterators
            //      have a `reference` that is either
            //      value_type& or value_type const&. (...)
            //
            //   The consequence of (1) is pretty significant - it means that
            //   if you have an iterator that returns a prvalue (...),
            //   it can never be stronger than an input iterator.
            //
            //   So, std::ranges::views::iota(1, 10),
            //   despite easily being able to support random access,
            //   is only, at best, a C++98 input range.
            //
            //   However, you can't just... remove this requirement.
            //   Existing code that assumes C++98 iterators
            //   and uses iterator_category to make judgements
            //
            //   is perfectly within its rights to assume that if
            //   iterator_category is, say, bidirectional_iterator_tag, that
            //   its `reference` is some kind of lvalue reference to value_type.
            //
            //   What iterator_concept does is add a new C++20 layer
            //   that allows an iterator to both advertise its C++98/17 category
            //   and, distinctly, advertise its C++20 category.
            //
            //   So going back to the std::ranges::iota_view<int, int> example,
            //   that view's iterator has iterator_category set to
            //   input_iterator_tag (because the `reference` is a prvalue and so
            //   it does not satisfy the old requirements for even forward),
            //
            //   but its iterator_concept is set to random_access_iterator_tag
            //   (because once we drop that restriction,
            //   we can easily support all the random access restrictions).
            //
            // ~ https://stackoverflow.com/questions/67606563
            //
            //
            // [C++ reference] - Iterator library: std::forward_iterator concept
            //
            // Notes
            //     Unlike the LegacyForwardIterator requirements,
            //     the std::forward_iterator concept
            //     does not require dereference to return an lvalue.
            //
            // ~ https://en.cppreference.com/w/cpp/iterator/forward_iterator
            //
            //
            // [C++ reference] - C++ named requirements: LegacyForwardIterator
            //
            // ~ https://en.cppreference.com/w/cpp/named_req/ForwardIterator
            //
            using iterator_concept  = std::random_access_iterator_tag;
            using iterator_category = std::        input_iterator_tag;

            using      value_type   =  element_type;
            using difference_type   = iterdiff_type;

            constexpr
            iterator () noexcept
            :
                value { 0 }
            { }

        private:
            constexpr explicit
            iterator (const value_type value) noexcept
            :
                value { value }
            { }

            friend constexpr
            auto iota_view::begin () const noexcept -> iterator;

            friend constexpr
            auto iota_view::  end () const noexcept -> iterator;

        public:
            constexpr
            auto operator * () const noexcept -> value_type
            {
                return value;
            }

            constexpr
            auto operator ++ () noexcept -> iterator&
            {
                ++value;
                return *this;
            }

            constexpr
            auto operator ++ (int) noexcept -> iterator
            {
                auto old = *this;
                ++*this;
                return old;
            }

            constexpr
            auto operator -- () noexcept -> iterator&
            {
                --value;
                return *this;
            }

            constexpr
            auto operator -- (int) noexcept -> iterator
            {
                auto old = *this;
                --*this;
                return old;
            }

            constexpr
            auto operator += (difference_type offset) noexcept -> iterator&
            {
                if constexpr (sizeof(element_type) >= sizeof(iterdiff_type))
                {
                    // note: Adding the iterator's value to the offset,
                    //       converted to the element_type,
                    //       will correctly advance this iterator, since:
                    //
                    //       (1) In the case, when
                    //           the element_type is a signed integral type,
                    //
                    //           the offset can be converted safely,
                    //           because the element_type and the iterdiff_type
                    //           are both signed integral types and
                    //           the element_type is wider.
                    //
                    //           The addition will not overflow,
                    //           since accessing out-of-range values is invalid,
                    //           that is, it is assumed that the addition will
                    //           return a value within the [init, bound] range:
                    //
                    //               init <= value + offset <= bound
                    //
                    //           In other words, it is expected that clients
                    //           will not specify incorrect offset values.
                    //
                    //       (2) Interestingly in the case, when
                    //           the element_type is an unsigned integral type,
                    //
                    //           the addition will also return a value
                    //           within the [init, bound] range,
                    //
                    //           that is, an arithmetically correct result,
                    //           even in the case, when the offset is negative,
                    //           as long as bits representing that offset
                    //           will be interpreted as an unsigned integer.
                    //
                    //           Yes, adding two n-bit unsigned integers
                    //           returns an unsigned integer value, equal to:
                    //
                    //            (a) x+y, when the result is
                    //                     less than or equal to uint_max,
                    //                     that is (x+y <= uint_max),
                    //
                    //            (b) x+y - 2^n, when the result is
                    //                           larger than uint_max,
                    //                           that is (x+y > uint_max),
                    //                           due to wraparound behavior of
                    //                           the modular arithmetic.
                    //
                    //           However, the operation of interpreting bits of
                    //           a signed integer as an unsigned integral value,
                    //
                    //         -2^(n-1) 2^(n-2)            4       2       1
                    //         +-------+-------+--   --+-------+-------+-------+
                    //         |       |       |  ...  |       |       |       |
                    //         +-------+-------+--   --+-------+-------+-------+
                    //         +2^(n-1) 2^(n-2)            4       2       1
                    //
                    //           changes (only) how the most significant bit
                    //           (MSB) is interpreted, that is, its weight
                    //           becomes positive, instead of being negative,
                    //
                    //           due to a change in value encoding,
                    //           from two's complement to binary.
                    //
                    //           In other words, casting
                    //           from a signed integer to an unsigned integer
                    //
                    //           subtracts -2^(n-1) from and adds +2^(n-1) to
                    //           values in range [int_min, 0) and
                    //           returns values in range [0, int_max] unchanged.
                    //
                    //           This effectively means, that adding
                    //           an unsigned integer 'x' and a signed integer 'y'
                    //           yields an arithmetically correct result,
                    //
                    //           as long as, that signed integer
                    //           is interpreted as an unsigned integer
                    //
                    //           and the addition result will fit into
                    //           the range of values representable by
                    //           that unsigned integer type,
                    //           to which the casting operation converts to:
                    //
                    //            (a) (y >= 0) - signed integer is non-negative,
                    //              (MSB == 0)   thus casting behaves as
                    //                           an identity function and
                    //                           the addition does not wraparound,
                    //                           yielding x+y,
                    //                           since x+y <= uint_max
                    //
                    //            (b) (y <  0) - signed integer is negative,
                    //              (MSB == 1)   hence casting returns y+2^n
                    //                           and the addition does wraparound,
                    //                           yielding exactly x+(y+2^n)-2^n,
                    //                           since x+y >= uint_min
                    //
                    //                           x+y     >= 0         guaranteed
                    //                           x+y+2^n >= 2^n       wraparound
                    //                           x+y+2^n >= uint_max+1    |
                    //                           x+y+2^n >  uint_max   <--+
                    //
                    //                x + y -(-2^(n-1)) +(+2^(n-1)) -2^n == x+y
                    //
                    //           In the case of iterator's value and the offset,
                    //           their sum is clearly within the range of values
                    //           representable by the element_type,
                    //
                    //           because it is assumed that iterators' values
                    //           are always within the [init, bound] range:
                    //
                    //           element_min <=
                    //                  init <= value + offset <= bound
                    //                                         <= element_max
                    //
                    value += static_cast<element_type>(offset);
                }
                else
                {
                    // note: Converting iterator's value to the iterdiff_type,
                    //       in order to perform the addition operation
                    //       with precision of the iterdiff_type,
                    //       and casting the result back to the element_type
                    //       will advance this iterator correctly, since:
                    //
                    //       (1) Any iterator's value, regardless of whether
                    //           the element_type is signed or unsigned,
                    //
                    //           can be converted, without any loss,
                    //           to the iterdiff_type,
                    //           since the iterdiff_type is wider.
                    //
                    //       (2) The addition will not overflow,
                    //           since accessing out-of-range values is invalid,
                    //
                    //           that is, it is assumed that iterators' values
                    //           are always within the [init, bound] range:
                    //
                    //               init <= value + offset <= bound
                    //
                    //           In other words, it is expected that clients
                    //           will not violate the above precondition,
                    //           by specifying incorrect offset values.
                    //
                    //       (3) Lastly, casting back the result
                    //           to the element_type is also correct, because
                    //
                    //           both the element_type and the iterdiff_type
                    //           are able to represent value of any iterator,
                    //
                    //               iterdiff_min <  element_min <= init
                    //               iterdiff_max >= element_max >= bound
                    //
                    //           including the value of this iterator
                    //           after the advance operation.
                    //
                    value =
                    static_cast<element_type>(iterdiff_type { value } + offset);
                }

                return *this;
            }

            constexpr
            auto operator -= (difference_type offset) noexcept -> iterator&
            {
                // note: Negating the offset will not cause any problems,
                //       since the range of valid offset values is symmetric,
                //
                //       that is, the result of negating a value
                //       in range [-iterdiff_max, +iterdiff_max]
                //       will always stay within that range.
                //
                *this += -offset;
                return *this;
            }

            constexpr
            auto operator [] (difference_type offset) const noexcept -> value_type
            {
                auto iter = *this;
                iter += offset;
                return *iter;
            }

            friend constexpr
            auto operator + (iterator        iter,
                             difference_type offset) noexcept -> iterator
            {
                iter += offset;
                return iter;
            }

            friend constexpr
            auto operator + (difference_type offset,
                             iterator        iter) noexcept -> iterator
            {
                iter += offset;
                return iter;
            }

            friend constexpr
            auto operator - (iterator        iter,
                             difference_type offset) noexcept -> iterator
            {
                iter -= offset;
                return iter;
            }

            friend constexpr
            auto operator - (const iterator& left,
                             const iterator& right) noexcept -> difference_type
            {
                if constexpr (sizeof(element_type) >= sizeof(iterdiff_type))
                {
                    // note: Converting, to the iterdiff_type,
                    //       the result of subtracting iterators' values
                    //       will compute the distance between these iterators
                    //       correctly, because:
                    //
                    //       (1) In the case, when
                    //           the element_type is a signed integral type,
                    //
                    //           the subtraction will return a value
                    //           in range [-iterdiff_max, +iterdiff_max],
                    //           since the size of the largest possible range
                    //           is less than or equal to the iterdiff_max.
                    //
                    //           This range of values is representable
                    //           by both the element_type and the iterdiff_type,
                    //
                    //           because the iterdiff_type can represent values
                    //           in range [-iterdiff_max-1, +iterdiff_max],
                    //           and the element_type is at least as wide as
                    //           the iterdiff_type, while also being signed.
                    //
                    //       (2) Interestingly in the case, when
                    //           the element_type is an unsigned integral type,
                    //
                    //           the subtraction will also return a value
                    //           in range [-iterdiff_max, +iterdiff_max],
                    //
                    //           that is, an arithmetically correct result,
                    //           even in the case, when the result is negative,
                    //           as long as bits representing that result
                    //           will be interpreted as a signed integer.
                    //
                    //           Yes, subtracting two n-bit unsigned integers
                    //           returns an unsigned integer value, equal to:
                    //
                    //            (a) x-y, when the result is non-negative,
                    //                     that is (x-y >= 0),
                    //
                    //            (b) x-y + 2^n, when the result is negative,
                    //                           that is (x-y < 0),
                    //                           due to wraparound behavior of
                    //                           the modular arithmetic.
                    //
                    //           However, the operation of interpreting bits of
                    //           an unsigned integer as a signed integral value,
                    //
                    //         +2^(n-1) 2^(n-2)            4       2       1
                    //         +-------+-------+--   --+-------+-------+-------+
                    //         |       |       |  ...  |       |       |       |
                    //         +-------+-------+--   --+-------+-------+-------+
                    //         -2^(n-1) 2^(n-2)            4       2       1
                    //
                    //           changes (only) how the most significant bit
                    //           (MSB) is interpreted, that is, its weight
                    //           becomes negative, instead of being positive,
                    //
                    //           due to a change in value encoding,
                    //           from binary to two's complement.
                    //
                    //           In other words, casting
                    //           from an unsigned integer to a signed integer
                    //
                    //           subtracts +2^(n-1) from and adds -2^(n-1) to
                    //           values in range (int_max, uint_max] and
                    //           returns values in range [0, int_max] unchanged.
                    //
                    //           This effectively means, that
                    //           subtracting two unsigned integers yields
                    //           an arithmetically correct result, as long as,
                    //           that result is interpreted as a signed integer:
                    //
                    //            (a) (x-y >= 0) - the result is non-negative,
                    //                (MSB == 0)   thus subtraction yields x-y
                    //                             and casting behaves as
                    //                             an identity function
                    //
                    //            (b) (x-y <  0) - the result is negative,
                    //                (MSB == 1)   hence the combination of
                    //                             subtraction and casting
                    //                             returns exactly x-y
                    //
                    //                x-y + 2^n -(+2^(n-1)) +(-2^(n-1)) == x-y
                    //
                    //           Obviously, the subtraction result must also
                    //           fit into the range of values
                    //           representable by the signed integer type,
                    //           to which the casting operation converts to.
                    //
                    //           This condition is also satisfied,
                    //           since the size of the largest possible range
                    //           is less than or equal to iterdiff_max.
                    //
                    return static_cast<iterdiff_type>(left.value - right.value);
                }
                else
                {
                    // note: Converting iterators' values to the iterdiff_type,
                    //       and then performing the subtraction,
                    //       will compute the distance between these iterators
                    //       correctly, because:
                    //
                    //       (1) Assuming the element_type is signed,
                    //           any value of an iterator
                    //           will fit into the iterdiff_type,
                    //
                    //           since the element_type and the iterdiff_type
                    //           are both signed integral types and
                    //           the iterdiff_type is wider.
                    //
                    //       (2) Assuming the element_type is unsigned,
                    //           any value of an iterator can be converted,
                    //           without any loss, to the iterdiff_type,
                    //
                    //           since in the worst case an iterator's value
                    //           will be equal to the uint_max, that is 2^(n)-1,
                    //
                    //           the iterdiff_type can represent all
                    //           non-negative values smaller than or equal to
                    //           the iterdiff_max, that is 2^(k-1)-1,
                    //
                    //           and n < k, or equivalently n <= k-1.
                    //
                    //           uint_max = 2^(n)-1 <= 2^(k-1)-1 = iterdiff_max
                    //
                    //       (3) The subtract operation will not overflow,
                    //
                    //           since the size of the largest possible range
                    //           is less than or equal to the iterdiff_max,
                    //
                    //           meaning that, the subtraction will return
                    //           a value in range [-iterdiff_max, +iterdiff_max],
                    //
                    //           which clearly fits in the range of values
                    //           representable by the iterdiff_type.
                    //
                    return
                    iterdiff_type { left.value } - iterdiff_type { right.value };
                }
            }

            friend constexpr
            auto operator == (const iterator& left,
                              const iterator& right) noexcept -> bool
            {
                return left.value == right.value;
            }

            friend constexpr
            auto operator != (const iterator& left,
                              const iterator& right) noexcept -> bool
            {
                return left.value != right.value;
            }

            friend constexpr
            auto operator < (const iterator& left,
                             const iterator& right) noexcept -> bool
            {
                return left.value < right.value;
            }

            friend constexpr
            auto operator > (const iterator& left,
                             const iterator& right) noexcept -> bool
            {
                return left.value > right.value;
            }

            friend constexpr
            auto operator <= (const iterator& left,
                              const iterator& right) noexcept -> bool
            {
                return left.value <= right.value;
            }

            friend constexpr
            auto operator >= (const iterator& left,
                              const iterator& right) noexcept -> bool
            {
                return left.value >= right.value;
            }
        };

    private:
        element_type  init;
        element_type bound;

    public:
        // note: There cannot exist an instance of the cxx::iota_view class,
        //       which size() exceeds the iterdiff_max limit,
        //
        //       since cxx::iota_view models std::ranges::random_access_range,
        //       and therefore must permit:
        //
        //        - advancing iterators across the whole range, and
        //        - computing the distance between any two iterators.
        //
        //       Specifically, advancing the end() iterator,
        //       by the offset equal to the negative size() of the range,
        //       must result in an iterator equal to the begin() iterator:
        //
        //          auto  iter     = std::ranges::  end(random_access_range);
        //          using iterdiff = std::iter_difference_t<decltype(iter)>;
        //          const
        //          auto  size     = std::ranges::ssize(random_access_range);
        //
        //          std::ranges::advance(iter, -static_cast<iterdiff>(size));
        //
        //          assert(iter  ==  std::ranges::begin(random_access_range));
        //
        //       Furthermore, computing the distance between
        //       the begin() iterator and the end() iterator
        //       must return a value equal to the negative size() of the range:
        //
        //          auto const distance =
        //               std::ranges::distance(std::ranges::begin(sized_range),
        //                                     std::ranges:: end (sized_range));
        //
        //          assert(distance == -std::ranges::ssize(sized_range));
        //
        //       In order to satisfy the above requirements,
        //       the iterdiff_type must be able to represent all values
        //       in the range [-size(), +size()].
        //
        //       This effectively requires the iterdiff_type
        //       to be defined as a signed integral type and
        //       limits the maximum size() of a range to the iterdiff_max.
        //
        constexpr
        iota_view (const element_type init, const element_type bound) noexcept
        :
            init { init }, bound { bound }
        {
            assert(init <= bound);

            constexpr
            auto iterdiff_max = std::numeric_limits<iterdiff_type>::max();

            if constexpr (sizeof(element_type) >= sizeof(iterdiff_type))
            {
                // note: A positive overflow while subtracting 'init' from 'bound'
                //       means that the range_size is greater than iterdiff_max.
                //
                if constexpr (std::is_signed_v<element_type>)
                {
                    assert(!cxx::detect_sub_positive_overflow(bound, init));
                }

                // note: It's safe to subtract 'init' from 'bound'
                //       to compute the 'range_size', because:
                //
                //       (1) the subtraction always yields a non-negative number
                //           that is, it will never cause
                //           a negative overflow or a negative wraparound,
                //           since init <= bound, meaning bound - init >= 0,
                //
                //       (2) the potential positive overflow,
                //           possible only when subtracting signed integers,
                //           has already been detected above.
                //
                const auto range_size = static_cast<element_type>(bound - init);
                //
                // note: In C++, arithmetic operations on integral types
                //       smaller than int, such as signed char or short int,
                //       perform integral promotions.
                //
                //       Since the subtraction will not overflow nor wraparound,
                //       integral promotions can be rejected, by casting
                //       the result to the source type of the arguments.
                //
                //       [C++ reference] Implicit conversions: Integral promotion
                //       ~ https://en.cppreference.com/w/cpp/language/implicit_conversion#integral_promotion

                assert(std::cmp_less_equal(range_size, iterdiff_max));
                //
                // note: The above comparison is heterogeneous, because
                //       it handles integer values of potentially different types,
                //       the signedness and size of which may not match.
            }
            else
            {
                // note: It is impossible to create a range of n-bit integers,
                //       which size() is greater than iterdiff_max,
                //       as long as k > n, where k is the number of bits
                //       in the iterdiff_type's representation.
                //
                //       In C++20 signed integers are guaranteed
                //       to use the two's complement representation,
                //       and therefore iterdiff_max = 2^(k-1)-1.
                //
                //       [ISO C++] P1236R1: Alternative Wording for P0907R4
                //       ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1236r1.html
                //       [ISO C++] P0907R4: Signed Integers are Two's Complement
                //       ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0907r4.html
                //
                //       The number of values representable as an n-bit integer,
                //       regardless of their signedness, equals to 2^n.
                //
                //       However, since [init, bound) form a half-open range,
                //       the value 'bound' is always excluded from the range.
                //
                //       This effectively prohibits creating a range,
                //       which contains the maximum integer value, limiting
                //       the size of the largest possible range to 2^(n)-1.
                //
                //       In the worst case, n equals to k-1,
                //       because it's the largest possible value of n,
                //       which is still smaller than k.
                //
                //       range_size_max = 2^(n)-1 <= 2^(k-1)-1 = iterdiff_max
            }
        }

        constexpr auto begin () const noexcept -> iterator
        {
            return iterator { init };
        }

        constexpr auto end () const noexcept -> iterator
        {
            return iterator { bound };
        }
    };
}


// [Tristan Brindle] - Rvalue Ranges and Views in C++20
//
// ~ https://tristanbrindle.com/posts/rvalue-ranges-and-views
//
template <typename  element_type,
          typename iterdiff_type>
inline constexpr
auto std::ranges::enable_borrowed_range<cxx::iota_view<element_type,
                                                      iterdiff_type>> = true;


namespace cxx
{
    struct iota_func
    {
        template <std::integral element_type>
        [[nodiscard]]
        constexpr auto operator () (const element_type init,
                                    const element_type bound) const noexcept
        {
            return iota_view { init, bound };
        }
    };

    inline constexpr auto iota = iota_func { };
}


#endif
