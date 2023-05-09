
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


#ifndef CXX_DETECT_OVERFLOW
#define CXX_DETECT_OVERFLOW


#include <concepts>

#include <limits>


namespace cxx
{
    // note: Adding two signed integers causes positive overflow, when:
    //
    //         left_value + right_value > signed_integral_max
    //
    //         left_value > signed_integral_max - right_value
    //
    //       1) (right_value <= 0) - positive overflow cannot occur,
    //                               because adding, in the worst case,
    //                               the signed_integral_max
    //                               to a non-positive value always yields
    //                               a value less than or equal to
    //                               the signed_integral_max
    //
    //       2) (right_value  > 0) - value (signed_integral_max - right_value)
    //                               can be safely computed and
    //                               compared against the left_value
    //                               to detect a potential positive overflow,
    //                               when adding two signed integers
    //
    template <std::signed_integral signed_integral>
    constexpr
    auto detect_add_positive_overflow (const signed_integral  left_value,
                                       const signed_integral right_value)
                                                                noexcept -> bool
    {
        constexpr
        auto signed_integral_max = std::numeric_limits<signed_integral>::max();

        return
        (right_value > 0) && (left_value > signed_integral_max - right_value);
    }


    // note: Adding two signed integers causes negative overflow, when:
    //
    //         left_value + right_value < signed_integral_min
    //
    //         left_value < signed_integral_min - right_value
    //
    //       1) (right_value >= 0) - negative overflow cannot occur,
    //                               because adding, in the worst case,
    //                               the signed_integral_min
    //                               to a non-negative value always yields
    //                               a value greater than or equal to
    //                               the signed_integral_min
    //
    //       2) (right_value  < 0) - value (signed_integral_min - right_value)
    //                               can be safely computed and
    //                               compared against the left_value
    //                               to detect a potential negative overflow,
    //                               when adding two signed integers
    //
    template <std::signed_integral signed_integral>
    constexpr
    auto detect_add_negative_overflow (const signed_integral  left_value,
                                       const signed_integral right_value)
                                                                noexcept -> bool
    {
        constexpr
        auto signed_integral_min = std::numeric_limits<signed_integral>::min();

        return
        (right_value < 0) && (left_value < signed_integral_min - right_value);
    }


    // note: Subtracting two signed integers causes positive overflow, when:
    //
    //         left_value - right_value > signed_integral_max
    //
    //         left_value > signed_integral_max + right_value
    //
    //       1) (right_value >= 0) - positive overflow cannot occur,
    //                               because subtracting a non-negative value
    //                               from, in the worst case,
    //                               the signed_integral_max always yields
    //                               a value less than or equal to
    //                               the signed_integral_max
    //
    //       2) (right_value  < 0) - value (signed_integral_max + right_value)
    //                               can be safely computed and
    //                               compared against the left_value
    //                               to detect a potential positive overflow,
    //                               when subtracting two signed integers
    //
    template <std::signed_integral signed_integral>
    constexpr
    auto detect_sub_positive_overflow (const signed_integral  left_value,
                                       const signed_integral right_value)
                                                                noexcept -> bool
    {
        constexpr
        auto signed_integral_max = std::numeric_limits<signed_integral>::max();

        return
        (right_value < 0) && (left_value > signed_integral_max + right_value);
    }


    // note: Subtracting two signed integers causes negative overflow, when:
    //
    //         left_value - right_value < signed_integral_min
    //
    //         left_value < signed_integral_min + right_value
    //
    //       1) (right_value <= 0) - negative overflow cannot occur,
    //                               because subtracting a non-positive value
    //                               from, in the worst case,
    //                               the signed_integral_min always yields
    //                               a value greater than or equal to
    //                               the signed_integral_min
    //
    //       2) (right_value  > 0) - value (signed_integral_min + right_value)
    //                               can be safely computed and
    //                               compared against the left_value
    //                               to detect a potential negative overflow,
    //                               when subtracting two signed integers
    //
    template <std::signed_integral signed_integral>
    constexpr
    auto detect_sub_negative_overflow (const signed_integral  left_value,
                                       const signed_integral right_value)
                                                                noexcept -> bool
    {
        constexpr
        auto signed_integral_min = std::numeric_limits<signed_integral>::min();

        return
        (right_value > 0) && (left_value < signed_integral_min + right_value);
    }
}


#endif
