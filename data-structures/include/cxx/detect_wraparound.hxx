
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


#ifndef CXX_DETECT_WRAPAROUND
#define CXX_DETECT_WRAPAROUND


#include <concepts>

#include <limits>


namespace cxx
{
    // note: Adding two unsigned integers causes positive wraparound, when:
    //
    //         left_value + right_value > unsigned_integral_max
    //
    //         left_value > unsigned_integral_max - right_value
    //
    //       1) (right_value == 0) - positive overflow cannot occur,
    //                               because adding, in the worst case,
    //                               the signed_integral_max to 0
    //                               always yields the signed_integral_max
    //
    //       2) (right_value  > 0) - value (unsigned_integral_max - right_value)
    //                               can be safely computed and
    //                               compared against the left_value
    //                               to detect a potential positive wraparound,
    //                               when adding two unsigned integers
    //
    template <std::unsigned_integral unsigned_integral>
    constexpr
    auto detect_add_positive_wraparound (const unsigned_integral  left_value,
                                         const unsigned_integral right_value)
                                                                noexcept -> bool
    {
        constexpr
        auto unsigned_integral_max = std::numeric_limits<unsigned_integral>::max();

        return
        (right_value != 0) && (left_value > unsigned_integral_max - right_value);
    }


    // note: Subtracting two unsigned integers causes negative wraparound, when:
    //
    //         left_value - right_value < 0
    //
    //         left_value < right_value
    //
    template <std::unsigned_integral unsigned_integral>
    constexpr
    auto detect_sub_negative_wraparound (const unsigned_integral  left_value,
                                         const unsigned_integral right_value)
                                                                noexcept -> bool
    {
        return left_value < right_value;
    }
}


#endif
