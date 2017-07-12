
#include <limits>
#include <cassert>
#include <type_traits>

namespace meta
{
	template <int N>
	struct Abs
	{
		static_assert(N != std::numeric_limits<int>::max(), "Out of range!");

		static constexpr auto value = (N > 0) ? N : -N;
	};

	constexpr int abs(const int n)
	{
		assert(n != std::numeric_limits<int>::max());

		return (n > 0) ? n : -n;
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <int Base, int Exponent>
	struct Power
	{
		static_assert(Exponent >= 0, "Fractions are not allowed!");

		static constexpr auto value = Power<Base, Exponent - 1>::value * Base;
	};

	template <int Base>
	struct Power <Base, 1>
	{
		static constexpr auto value = Base;
	};

	template <int Base>
	struct Power <Base, 0>
	{
		static_assert(Base != 0, "Indeterminate form are not allowed!");

		static constexpr auto value = 1;
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename Type, Type Constant>
	struct integral_constant
	{
		static constexpr auto value = Constant;
	};

	template <bool Boolean>
	using bool_constant = integral_constant <bool, Boolean>;

	using  true_type = bool_constant < true>;
	using false_type = bool_constant <false>;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename Type>
	struct remove_const
	{
		using type = Type;
	};

	template <typename Type>
	struct remove_const <const Type>
	{
		using type = Type;
	};

	template <typename Type>
	using remove_const_t = typename remove_const<Type>::type;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename Type>
	struct remove_volatile
	{
		using type = Type;
	};

	template <typename Type>
	struct remove_volatile <volatile Type>
	{
		using type = Type;
	};

	template <typename Type>
	using remove_volatile_t = typename remove_volatile<Type>::type;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename Type>
	using remove_cv = remove_volatile<typename remove_const<Type>::type>;

	template <typename Type>
	using remove_cv_t = typename remove_cv<Type>::type;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <bool Condition, typename ThenType, typename ElseType>
	struct conditional;

	template <typename ThenType, typename ElseType>
	struct conditional<true, ThenType, ElseType>
	{
		using type = ThenType;
	};

	template <typename ThenType, typename ElseType>
	struct conditional<false, ThenType, ElseType>
	{
		using type = ElseType;
	};

	template <bool Condition, typename ThenType, typename ElseType>
	using conditional_t = typename conditional<Condition, ThenType, ElseType>::type;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <bool Condition, typename Type>
	struct enable_if;

	template <typename Type>
	struct enable_if <true, Type>
	{
		using type = Type;
	};

	template <typename Type>
	struct enable_if <false, Type>
	{
	};

	template <bool Condition, typename Type>
	using enable_if_t = typename enable_if<Condition, Type>::type;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	/* * * * * * * * * * * * * * * * * * * * * * * * * *
	 * SFINAE --> Substitution Failure Is Not An Error *
	 * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <typename T>
	enable_if_t<std::is_integral<T>::value, char>
	foo (const T)
	{
		// overload for integral types
		return 'i';
	}

	template <typename T>
	enable_if_t<std::is_floating_point<T>::value, char>
	foo (const T)
	{
		// overload for floating point types
		return 'f';
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename FirstType, typename SecondType>
	struct is_same : false_type { };

	template <typename Type>
	struct is_same <Type, Type> : true_type { };

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename Type>
	using is_void = is_same<typename remove_cv<Type>::type, void>;
}

namespace assert
{
	template <typename FirstType, typename SecondType>
	struct same_types
	{
		static_assert
		(
			std::is_same<FirstType, SecondType>::value,
			"Types have to be the same!"
		);
	};

	template <auto FirstValue, auto SecondValue>
	struct equal_values
	{
		static_assert
		(
			FirstValue == SecondValue,
			"Values have to be the equal!"
		);
	};
}

int main()
{
	assert::equal_values<meta::Abs<-9>::value, 9>();
	assert::equal_values<meta::Abs<+7>::value, 7>();

	assert::equal_values<meta::abs(-9), 9>();
	assert::equal_values<meta::abs(+7), 7>();

	assert::equal_values<meta::Power< 2, 8>::value, 256>();
	assert::equal_values<meta::Power<-5, 3>::value,-125>();
	assert::equal_values<meta::Power< 0, 3>::value,   0>();
	assert::equal_values<meta::Power< 3, 0>::value,   1>();
	assert::equal_values<meta::Power<11, 1>::value,  11>();

	assert::equal_values<meta::integral_constant<int, 7>::value, 7>();
	assert::same_types<decltype(meta::integral_constant<char, '$'>::value), const char>();

	assert::same_types<meta::remove_const_t<const    int>,          int>();
	assert::same_types<meta::remove_const_t<volatile int>, volatile int>();

	assert::same_types<meta::remove_volatile_t<volatile int>,       int>();
	assert::same_types<meta::remove_volatile_t<const    int>, const int>();

	assert::same_types<meta::remove_cv_t<               int>,       int>();
	assert::same_types<meta::remove_cv_t<      volatile int>,       int>();
	assert::same_types<meta::remove_cv_t<const          int>,       int>();
	assert::same_types<meta::remove_cv_t<const volatile int>,       int>();

	assert::same_types<meta::conditional_t< true, char, long>, char>();
	assert::same_types<meta::conditional_t<false, char, long>, long>();

	assert::same_types<meta::enable_if_t<true, char>, char>();
	assert(meta::foo(5)   == 'i');
	assert(meta::foo(5.0) == 'f');

	assert::equal_values<meta::is_same<char, char>::value,  true>();
	assert::equal_values<meta::is_same<char, long>::value, false>();

	assert::equal_values<meta::is_void<               void>::value,  true>();
	assert::equal_values<meta::is_void<const          void>::value,  true>();
	assert::equal_values<meta::is_void<      volatile void>::value,  true>();
	assert::equal_values<meta::is_void<const volatile void>::value,  true>();
	assert::equal_values<meta::is_void<               long>::value, false>();

	return 0;
}
