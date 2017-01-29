
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, mtezych
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

#include <cstdint>
#include <limits>

//
// http://www.csee.umbc.edu/~tsimo1/CMSC455/IEEE-754-2008.pdf
// https://en.wikipedia.org/wiki/IEEE_754-1985
//
namespace ieee754
{
	enum class Float
	{
		PositiveZero,
		NegativeZero,
		DenormalizedNumber,
		PositiveInfinity,
		NegativeInfinity,
		NotANumber,
		NormalizedNumber,
	};

	Float floatType (const float number)
	{
		static_assert
		(
			(sizeof(float) == sizeof(uint32_t)) && std::numeric_limits<float>::is_iec559,
			"Single precision IEEE 754 floating point number has to be 32-bit."
		);

		const auto bits         = *reinterpret_cast<const uint32_t* const>(&number);

		/*                                     *\
		|         IEEE 754 float 32-bit         |
		|                                       |
		| 31 30    23 22                     0  |
		| +-+--------+------------------------+ |
		| | |exponent|        fraction        | |
		| +-+--------+------------------------+ |
		|  ^                                    |
		| sign                                  |
		\*                                     */

		const auto sign         = uint32_t { bits & 0x80000000 } >> 31;
		const auto exponentBias = uint32_t { bits & 0x7F800000 } >> 23;
		const auto fraction     = uint32_t { bits & 0x007FFFFF } >>  0;

		if (exponentBias == 0x0)
		{
			if (fraction == 0x0)
			{
				return (sign == 0x1) ? Float::NegativeZero : Float::PositiveZero;
			}
			else
			{
				return Float::DenormalizedNumber;
			}
		}
		else if (exponentBias == 0xFF)
		{
			if (fraction == 0x0)
			{
				return (sign == 0x1) ? Float::NegativeInfinity : Float::PositiveInfinity;
			}
			else
			{
				return Float::NotANumber;
			}
		}
		else
		{
			return Float::NormalizedNumber;
		}
	}
}

#include <cassert>

void test (const float value, const ieee754::Float type)
{
	assert(ieee754::floatType(value) == type);
}

int main ()
{
	static_assert(std::numeric_limits<float>::has_infinity     , "IEEE 754 requires infinity");
	static_assert(std::numeric_limits<float>::has_quiet_NaN    , "IEEE 754 requires qNaN");
	static_assert(std::numeric_limits<float>::has_signaling_NaN, "IEEE 754 requires sNaN");

	test(+0.0f                                      , ieee754::Float::PositiveZero);
	test(-0.0f                                      , ieee754::Float::NegativeZero);

	test(std::numeric_limits<float>::min()          , ieee754::Float::NormalizedNumber);
	test(std::numeric_limits<float>::max()          , ieee754::Float::NormalizedNumber);
	test(std::numeric_limits<float>::lowest()       , ieee754::Float::NormalizedNumber);

	test(std::numeric_limits<float>::quiet_NaN()    , ieee754::Float::NotANumber);
	test(std::numeric_limits<float>::signaling_NaN(), ieee754::Float::NotANumber);

	test(+std::numeric_limits<float>::infinity()    , ieee754::Float::PositiveInfinity);
	test(-std::numeric_limits<float>::infinity()    , ieee754::Float::NegativeInfinity);

	test(std::numeric_limits<float>::denorm_min()   , ieee754::Float::DenormalizedNumber);

	return 0;
}
