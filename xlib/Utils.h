
#ifndef UTILS
#define UTILS

#include <type_traits>

namespace utility
{
	template <typename ArithmeticType>
	struct Size
	{
		static_assert
		(
			std::is_arithmetic<ArithmeticType>::value,
			"Arithmetic type is required."
		);
		ArithmeticType width;
		ArithmeticType height;
	};
}

#endif
