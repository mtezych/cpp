
#include <utility>
#include <type_traits>
#include <cstdint>
#include <iostream>
#include <array>

namespace foo
{
	template <typename Type>
	constexpr bool Moveable()
	{
		return	std::is_move_constructible<Type>::value &&
				std::is_move_assignable   <Type>::value;
	}

	template <typename Type>
	constexpr bool Destructible()
	{
		return std::is_destructible<Type>::value;
	}

	template <typename Type> requires Moveable<Type>() && Destructible<Type>()
	void swap (Type& left, Type& right)
	{
		auto temp = std::move(left);  // move constructor
		left      = std::move(right); // move assign operator
		right     = std::move(temp);
	}

	template <typename Type>
	concept constexpr bool Copyable()
	{
		return	std::is_copy_constructible<Type>::value &&
				std::is_copy_assignable   <Type>::value;
	}

	template <typename Type>
	concept bool Comparable = requires (Type left, Type right)
	{
		{ left <  right } -> bool;
		{ left >  right } -> bool;
		{ left >= right } -> bool;
		{ left <= right } -> bool;
		{ left == right } -> bool;
		{ left != right } -> bool;
	};

	template <Comparable Type>
	Type max (const Type& left, const Type& right)
	{
		static_assert(Copyable<Type>(), "Type has to be Copyable.");
		if (true)
		{
			return (left > right) ? left : right; //   less than & copy
		}
		else
		{
			return (left < right) ? right : left; // grater than & copy
		}
	}

	template <typename Type, typename... ArgumentTypes>
	concept bool Callable = requires (Type callable, ArgumentTypes... arguments)
	{
		{ callable(arguments...) };
	};

	template <typename CallableType, typename... ArgumentTypes> 
	requires Callable<CallableType, ArgumentTypes...> 
	auto invoke(const CallableType& callable, ArgumentTypes&&... arguments)
	{
		return callable(std::forward<ArgumentTypes>(arguments)...);
	}

	template <uint8_t Element, uint8_t... Elements> // concepts not needed here
	struct reduce
	{
		static_assert
		(
			sizeof...(Elements) > 0,
			"Empty parameter pack should not happen. "
			"Partial specialisation with one element should be preffered."
		);
		constexpr static auto value = Element + reduce<Elements...>::value;
	};

	template <uint8_t Element>
	struct reduce<Element>
	{
		constexpr static auto value = Element; 
	};

	template <typename Type>
	concept bool Arithmetic = requires (Type arithmetic, Type other)
	{
		{ +arithmetic } -> Type;
		{ -arithmetic } -> Type;

		{ arithmetic + other } -> Type;
		{ arithmetic - other } -> Type;
		{ arithmetic * other } -> Type;
		{ arithmetic / other } -> Type;

		{ arithmetic = other } -> Type&;

		{ arithmetic += other } -> Type&;
		{ arithmetic -= other } -> Type&;
		{ arithmetic *= other } -> Type&;
		{ arithmetic /= other } -> Type&;

		{ Type { 0 } };
	};

	// WA: Allows handling empty parameter pack.
	constexpr auto accumulate ()
	{
		return 0;
	}

	// WA: This overload is not called.
	template <Arithmetic ElementType>
	constexpr auto accumulate (ElementType element)
	{
		return element;
	}

	template <Arithmetic ElementType, Arithmetic... ElementTypes>
	constexpr auto accumulate (ElementType element, ElementTypes... elements)
	{
		// WA: Overload resolution fails to choose overload with one element.
		//     Because of that reason empty parameter pack is generated.
		// static_assert
		// (
		// 	sizeof...(elements) > 0,
		// 	"Empty parameter pack should not happen. "
		// 	"Overload with one element should be preffered."
		// );
		return element + accumulate(elements...);
	}

	template <typename Type>
	concept bool Iterator = requires (Type iterator, Type other)
	{
		*iterator;
		{ ++iterator } -> Type&;
		{ iterator != other } -> bool;
	};

	template <typename Type>
	concept bool Container = requires (Type container)
	{
		typename Type::value_type;
		Iterator<typename Type::iterator>;
		{ container.begin() } -> typename Type::iterator;
		{ container.end()   } -> typename Type::iterator;
	};

	void sort(Container& container)
	{
		for (auto& elementA : container)
		{
			for (auto& elementB : container)
			{
				static_assert
				(
					Comparable<decltype(elementA)>,
					"Container's Elements have to be Comparable."
				);
				if (elementA < elementB)
				{
					swap(elementA, elementB);
				}
			}
		}
	}
}

namespace bar
{
	struct Bar
	{
		uint32_t b;

		bool operator <  (const Bar& bar) const { return this->b <  bar.b; }
		bool operator >  (const Bar& bar) const { return this->b >  bar.b; }
		bool operator <= (const Bar& bar) const { return this->b <= bar.b; }
		bool operator >= (const Bar& bar) const { return this->b >= bar.b; }
		bool operator == (const Bar& bar) const { return this->b == bar.b; }
		bool operator != (const Bar& bar) const { return this->b != bar.b; }
	};
}

namespace foo
{
	// WA: Explicit (full) template specialization does not work.
	//     Is overload resolution failing because
	//     template requirements are part of the interface?
	// template <>
	void swap /* <bar::Bar> */ (bar::Bar& left, bar::Bar& right)
	{
		left .b = left.b ^ right.b;
		right.b = left.b ^ right.b;
		left .b = left.b ^ right.b;
	}
}

int main()
{
	auto x = 1;
	auto y = 2;
	foo::swap(x, y);
	auto p = bar::Bar { 47 };
	auto q = bar::Bar { 31 };
	foo::swap(p, q);
	
	const auto a = foo::max(2, 4);
	const auto b = foo::max(1.2f, 1.3f);
	const auto c = foo::max(bar::Bar { 54 }, bar::Bar { 34 });

	const auto l = [](){};
	foo::invoke(l);
	const auto m = [](int, char) -> double { return 5.6; };
	const double d = foo::invoke(m, 45, '#');

	constexpr auto s = foo::reduce<8, 6, 2, 7>::value;
	constexpr auto t = foo::accumulate(8, 6, 2, 7);

	auto v = std::array<bar::Bar, 5> { 123, 53, 421, 7, 112 };
	foo::sort(v);

	return 0;
}
