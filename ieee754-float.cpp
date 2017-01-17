
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
