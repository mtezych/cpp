
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
	using remove_cv = remove_volatile<typename remove_const<Type>::type>;


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


	// SFINAE

	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, char>::type
	foo (const T)
	{
		// overload for integral types
		return 'i';
	}

	template <typename T>
	typename std::enable_if<std::is_floating_point<T>::value, char>::type
	foo (const T)
	{
		// overload for floating point types
		return 'f';
	}


	template <typename FirstType, typename SecondType>
	struct is_same
	{
		static constexpr auto value = false;
	};

	template <typename Type>
	struct is_same <Type, Type>
	{
		static constexpr auto value = true;
	};


	template <typename Type>
	using is_void = is_same<typename remove_cv<Type>::type, void>;
}

int main()
{
	static_assert(meta::Abs<-9>::value == 9, "");
	static_assert(meta::Abs<+7>::value == 7, "");

	static_assert(meta::abs(-9) == 9, "");
	static_assert(meta::abs(+7) == 7, "");

	static_assert(meta::Power< 2, 8>::value ==  256, "");
	static_assert(meta::Power<-5, 3>::value == -125, "");
	static_assert(meta::Power< 0, 3>::value ==    0, "");
	static_assert(meta::Power< 3, 0>::value ==    1, "");
	static_assert(meta::Power<11, 1>::value ==   11, "");

	static_assert(std::is_same<meta::remove_const<const    int>::type,          int>::value, "");
	static_assert(std::is_same<meta::remove_const<volatile int>::type, volatile int>::value, "");

	static_assert(std::is_same<meta::remove_volatile<volatile int>::type,       int>::value, "");
	static_assert(std::is_same<meta::remove_volatile<const    int>::type, const int>::value, "");

	static_assert(std::is_same<meta::remove_cv<               int>::type,       int>::value, "");
	static_assert(std::is_same<meta::remove_cv<      volatile int>::type,       int>::value, "");
	static_assert(std::is_same<meta::remove_cv<const          int>::type,       int>::value, "");
	static_assert(std::is_same<meta::remove_cv<const volatile int>::type,       int>::value, "");

	static_assert(std::is_same<meta::conditional< true, char, long>::type, char>::value, "");
	static_assert(std::is_same<meta::conditional<false, char, long>::type, long>::value, "");

	static_assert(std::is_same<meta::enable_if<true, char>::type, char>::value, "");
	assert(meta::foo(5)   == 'i');
	assert(meta::foo(5.0) == 'f');

	static_assert( meta::is_same<char, char>::value, "");
	static_assert(!meta::is_same<char, long>::value, "");

	static_assert( meta::is_void<               void>::value, "");
	static_assert( meta::is_void<const          void>::value, "");
	static_assert( meta::is_void<      volatile void>::value, "");
	static_assert( meta::is_void<const volatile void>::value, "");
	static_assert(!meta::is_void<               long>::value, "");

	return 0;
}
