
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

// Constraints and concepts
// ~ http://en.cppreference.com/w/cpp/language/constraints

#include <utility>
#include <type_traits>
#include <cstdint>

namespace concepts
{
	template <typename Type>
	constexpr bool Moveable ()
	{
		return
		std::is_move_constructible<Type>::value &&
		std::is_move_assignable   <Type>::value;
	}

	template <typename Type>
	constexpr bool Destructible ()
	{
		return std::is_destructible<Type>::value;
	}

	template <typename Type> requires Moveable<Type>() && Destructible<Type>()
	void swap (Type& left, Type& right)
	{
		auto temp = std::move(left);
		left      = std::move(right);
		right     = std::move(temp);
	}


	template <typename Type>
	concept constexpr bool Copyable ()
	{
		return
		std::is_copy_constructible<Type>::value &&
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

		if constexpr (true)
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
	auto invoke (const CallableType& callable, ArgumentTypes&&... arguments)
	{
		return callable(std::forward<ArgumentTypes>(arguments)...);
	}


	template <auto Element, auto... Elements>
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

	template <auto Element>
	struct reduce <Element>
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

	template <Arithmetic ElementType, Arithmetic... ElementTypes>
	constexpr auto accumulate (ElementType element, ElementTypes... elements)
	{
		if constexpr (sizeof...(elements) > 0)
		{
			return element + accumulate(elements...);
		}
		else
		{
			return element;
		}
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

	void sort (Container& container)
	{
		for (auto& elementA : container)
		{
			for (auto& elementB : container)
			{
				static_assert
				(
					Comparable<decltype(elementA)>,
					"Containers' Elements have to be Comparable."
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
		uint32_t value;

		bool operator <  (const Bar& bar) const { return value <  bar.value; }
		bool operator >  (const Bar& bar) const { return value >  bar.value; }
		bool operator <= (const Bar& bar) const { return value <= bar.value; }
		bool operator >= (const Bar& bar) const { return value >= bar.value; }
		bool operator == (const Bar& bar) const { return value == bar.value; }
		bool operator != (const Bar& bar) const { return value != bar.value; }
	};

	void swap (bar::Bar& left, bar::Bar& right)
	{
		left .value = left.value ^ right.value;
		right.value = left.value ^ right.value;
		left .value = left.value ^ right.value;
	}
}


#include <array>
#include <cassert>

int main()
{
	{
		using concepts::swap;

		auto x = 1;
		auto y = 2;
		swap(x, y);
		assert(x == 2);
		assert(y == 1);

		auto a = bar::Bar { 47 };
		auto b = bar::Bar { 31 };
		swap(a, b);
		assert(a == bar::Bar { 31 });
		assert(b == bar::Bar { 47 });
	}

	{
		const auto i = concepts::max(2, 4);
		assert(i == 4);

		const auto f = concepts::max(1.2f, 1.3f);
		assert(f == 1.3f);

		const auto b = concepts::max(bar::Bar { 54 }, bar::Bar { 34 });
		assert(b == bar::Bar { 54 });
	}

	{
		const auto nop = [](){};
		concepts::invoke(nop);

		const auto lambda = [] (int, char) { return 5.6; };
		const auto d = double
		{
			concepts::invoke(lambda, 45, '#')
		};
		assert(d == 5.6);
	}

	{
		constexpr auto r = concepts::reduce<8, 6, 2, 7>::value;
		assert(r == 23);

		constexpr auto a = concepts::accumulate(8, 6, 2, 7);
		assert(a == 23);
	}

	{
		auto array = std::array<bar::Bar, 5> { 123, 53, 421, 7, 112 };
		concepts::sort(array);
	}

	return 0;
}
