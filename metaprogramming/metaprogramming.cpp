
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
	struct identity
	{
		using type = Type;
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename Type>
	struct remove_const : identity<Type> { };

	template <typename Type>
	struct remove_const <const Type> : identity<Type> { };

	template <typename Type>
	using remove_const_t = typename remove_const<Type>::type;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename Type>
	struct remove_volatile : identity<Type> { };

	template <typename Type>
	struct remove_volatile <volatile Type> : identity<Type> { };

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
	struct conditional<true, ThenType, ElseType> : identity<ThenType> { };

	template <typename ThenType, typename ElseType>
	struct conditional<false, ThenType, ElseType> : identity<ElseType> { };

	template <bool Condition, typename ThenType, typename ElseType>
	using conditional_t = typename conditional<Condition, ThenType, ElseType>::type;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <bool Condition, typename Type>
	struct enable_if;

	template <typename Type>
	struct enable_if <true, Type> : identity<Type> { };

	template <typename Type>
	struct enable_if <false, Type> { };

	template <bool Condition, typename Type>
	using enable_if_t = typename enable_if<Condition, Type>::type;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	/* * * * * * * * * * * * * * * * * * * * * * * * * *
	 * SFINAE --> Substitution Failure Is Not An Error *
	 * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <typename T>
	enable_if_t<std::is_integral<T>::value, char> foo (const T)
	{
		return 'i'; // overload for integral types
	}

	template <typename T>
	enable_if_t<std::is_floating_point<T>::value, char> foo (const T)
	{
		return 'f'; // overload for floating point types
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename FirstType, typename SecondType>
	struct is_same : false_type { };

	template <typename Type>
	struct is_same <Type, Type> : true_type { };

	template <typename FirstType, typename SecondType>
	constexpr auto is_same_v = is_same<FirstType, SecondType>::value;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	template <typename Type>
	using is_void = is_same<typename remove_cv<Type>::type, void>;

	template <typename Type>
	constexpr auto is_void_v = is_void<Type>::value;
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

	assert::equal_values<meta::is_same_v<char, char>,  true>();
	assert::equal_values<meta::is_same_v<char, long>, false>();

	assert::equal_values<meta::is_void_v<               void>,  true>();
	assert::equal_values<meta::is_void_v<const          void>,  true>();
	assert::equal_values<meta::is_void_v<      volatile void>,  true>();
	assert::equal_values<meta::is_void_v<const volatile void>,  true>();
	assert::equal_values<meta::is_void_v<               long>, false>();

	return 0;
}
