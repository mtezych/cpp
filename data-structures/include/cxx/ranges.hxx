
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


#ifndef CXX_RANGES
#define CXX_RANGES


#include <ranges>


namespace cxx::ranges
{
    template <typename type>
    concept sized_random_access_range = std::ranges::        sized_range<type>
                                     && std::ranges::random_access_range<type>;


    struct ssize_func
    {
        // [ISO C++] - Working Draft, C++20 Standard
        //
        //
        //  26.4.3 Sized ranges                              [range.sized]
        //
        //  (1) The sized_range concept refines range with the requirement
        //      that the number of elements in the range can be determined
        //      in amortized constant time using std::ranges::size().
        //
        //      template <class T>
        //      concept sized_range = range<T> && requires (T& t)
        //                                        {
        //                                            ranges::size(t);
        //                                        };
        //
        //  (2) Given an lvalue t of type remove_reference_t<T>,
        //      T models sized_range only if
        //
        //      - ranges::size(t) is amortized O(1), does not modify t,
        //        and is equal to ranges::distance(ranges::begin(t),
        //                                         ranges:: end (t))
        //
        //
        //  25.3.4.4 Concept weakly_incrementable      [iterator.concept.winc]
        //
        //  (6) Expressions of integer-class type
        //      are explicitly convertible to any integer-like type,
        //      and implicitly convertible to any integer-class type
        //      of equal or greater width and the same signedness.
        //
        //      Expressions of integral type are both
        //      implicitly and explicitly convertible to any integer-class type.
        //
        //      Conversions between integral and integer-class types and
        //      between two integer-class types do not exit via an exception.
        //
        //
        // ~ https://open-std.org/jtc1/sc22/wg21/docs/papers/2022/n4910.pdf

        template <std::ranges::sized_range sized_range>
        //
        [[nodiscard]]
        constexpr auto operator () (sized_range&& range) const
        //
        noexcept ( noexcept (  std::ranges::size(range)  ) )
        {
            const
            auto  range_size = std::ranges::size(range);

            using range_diff = std::ranges::range_difference_t<sized_range>;

            // note: The difference_type of a sized_range is guaranteed to
            //       be able to represent the maximum size of the sized_range,
            //
            //       because the std::ranges::sized_range concept requires
            //       the value returned by the std::ranges::size(sized_range)
            //       to be equal to the value returned by
            //       the std::ranges::distance(std::ranges::begin(sized_range),
            //                                 std::ranges:: end (sized_range)).
            //
            return static_cast<range_diff>(range_size);
            //
            // note: A conversion between integer-like types
            //       never throws an exception.
        }
    };

    inline constexpr auto ssize = ssize_func { };


    struct end_iter_func
    {
        template <std::ranges::common_range common_range>
        [[nodiscard]]
        constexpr auto operator () (common_range&& range) const
        noexcept(
        noexcept(  std::ranges::end(range)  ))
        {
            return std::ranges::end(range);
        }

        template <cxx::ranges::sized_random_access_range sized_random_access_range>
        requires
                             (!std::ranges::common_range<sized_random_access_range>)
        [[nodiscard]]
        constexpr auto operator () (sized_random_access_range&& range) const
        noexcept(
        noexcept(  std::ranges::begin(range) + cxx::ranges::ssize(range)  ))
        {
            return std::ranges::begin(range) + cxx::ranges::ssize(range);
        }
    };

    inline constexpr auto end_iter = end_iter_func { };


    // note: The cxx::simple_range concept specifies a range,
    //       which behaves identically, regardless of whether it has been passed
    //       to the std::ranges::begin() / std::ranges::end() CPO
    //       as a constant or a mutable value,
    //
    //       that is, in both cases the iterator / sentinel
    //       returned by the std::ranges::begin() / std::ranges::end() CPO
    //       will have the exact same type and value.
    //
    //
    //       This effectively means that a user-defined type,
    //       which models the cxx::simple_range concept must either:
    //
    //        - do not overload, that is, provide a single implementation,
    //          of the begin() and the end() member functions,
    //          which accept ranges both as constants and as mutable values, or
    //
    //        - implement consistently all overloads
    //          of the begin() and the end() member / free functions, such that
    //          they all return iterators or sentinels, which are equal.
    //
    //
    //  class simple_range                    class simple_range
    //  {                                     {
    //    func begin () const -> iterator;      func begin () const -> iterator;
    //    func  end  () const -> sentinel;      func begin ()       -> iterator;
    //  }                                       func  end  () const -> sentinel;
    //                                          func  end  ()       -> sentinel;
    //                                        }
    //
    //  class simple_range                    class simple_range
    //  {                                     {
    //    func begin () const -> iterator;      func begin () const -> iterator;
    //    func begin ()       -> iterator;      func  end  () const -> sentinel;
    //    func  end  () const -> sentinel;      func  end  ()       -> sentinel;
    //  }                                     }
    //
    //  class simple_range
    //  {
    //    friend func begin (const simple_range&) -> iterator;
    //    friend func begin (      simple_range&) -> iterator;
    //    friend func  end  (const simple_range&) -> sentinel;
    //    friend func  end  (      simple_range&) -> sentinel;
    //  }
    //
    //
    //       Note that, in order to satisfy both requirements:
    //        - std::ranges::range<      type>
    //        - std::ranges::range<const type>
    //       a user-defined type must provide overloads of
    //       the begin() and the end() free functions
    //       taking both const and mutable lvalue reference,
    //
    //       due to restricted lookup rules,
    //       taking into account candidates found *only* via ADL,
    //       which are specific to CPOs defined by the C++ ranges library,
    //       such as std::ranges::begin() and std::ranges::end().
    //
    //
    //       The fact that for a simple_range it does not matter
    //       whether the std::ranges::begin() and std::ranges::end() CPOs
    //       are invoked with a constant or a mutable value as their argument,
    //
    //       means that some of the C++ range adaptors,
    //       such as std::views::take() or std::views::zip(),
    //       provide non-const overload of the begin() member function
    //       only when the underlying (adapted) range is not a simple_range.
    //
    template <typename type>
    concept simple_range = std::ranges::range     <      type>   &&
                           std::ranges::range     <const type>
                                                                 &&
              std::same_as<std::ranges::iterator_t<      type>,
                           std::ranges::iterator_t<const type>>
                                                                 &&
              std::same_as<std::ranges::sentinel_t<      type>,
                           std::ranges::sentinel_t<const type>>;
}


#endif
