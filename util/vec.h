
//
// ISO C++ Standard
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4296.pdf
//
// 9   Classes
//
// 9.2 Class members
//
//  19 In a standard-layout union with an active member of struct type T1,
//     it is permitted to read a non-static data member m
//     of another union member of struct type T2
//     provided m is part of the common initial sequence of T1 and T2.
//
// 9.5 Unions
//
//   1 In a union, at most one of the non-static data members
//     can be active at any time, that is, the value of at most one of
//     the non-static data members can be stored in a union at any time.
//
//     [Note]
//     One special guarantee is made in order to simplify the use of unions:
//     If a standard-layout union contains several standard-layout structs
//     that share a common initial sequence,
//     and if an object of this standard-layout union type
//     contains one of the standard-layout structs,
//     it is permitted to inspect the common initial sequence
//     of any of standard-layout struct members.
//

#ifndef UTIL_VEC
#define UTIL_VEC

#include <type_traits>
#include <cassert>

namespace util
{
	template <typename ElemType>
	struct Vector4
	{
		static_assert
		(
			std::is_arithmetic<ElemType>::value,
			"ElemType has to be arithmetic."
		);

		union
		{
			ElemType x;
			ElemType r;
			ElemType s;
			ElemType red;
			ElemType width;
		};

		union
		{
			ElemType g;
			ElemType y;
			ElemType t;
			ElemType green;
			ElemType height;
		};

		union
		{
			ElemType b;
			ElemType z;
			ElemType p;
			ElemType blue;
			ElemType depth;
		};

		union
		{
			ElemType a;
			ElemType w;
			ElemType q;
			ElemType alpha;
			ElemType time;
		};

		Vector4 ()
		:
			x { ElemType { } },
			y { ElemType { } },
			z { ElemType { } },
			w { ElemType { } }
		{
		}

		Vector4
		(
			const ElemType x,
			const ElemType y,
			const ElemType z,
			const ElemType w
		):
			x { x },
			y { y },
			z { z },
			w { w }
		{
		}

		const ElemType& operator [] (const int index) const
		{
			assert(index >= 0 && index <= 3);

			if (index <= 0)
			{
				return x;
			}
			else if (index == 1)
			{
				return y;
			}
			else if (index == 2)
			{
				return z;
			}
			else // index >= 3
			{
				return w;
			}
		}

		ElemType& operator [] (const int index)
		{
			assert(index >= 0 && index <= 3);

			if (index <= 0)
			{
				return x;
			}
			else if (index == 1)
			{
				return y;
			}
			else if (index == 2)
			{
				return z;
			}
			else // index >= 3
			{
				return w;
			}
		}
	};

	using  vec4 = Vector4<float       >;
	using dvec4 = Vector4<double      >;
	using ivec4 = Vector4<int         >;
	using uvec4 = Vector4<unsigned int>;
	using bvec4 = Vector4<bool        >;

	static_assert(sizeof( vec4) == sizeof(float       ) * 4, "No padding.");
	static_assert(sizeof(dvec4) == sizeof(double      ) * 4, "No padding.");
	static_assert(sizeof(ivec4) == sizeof(int         ) * 4, "No padding.");
	static_assert(sizeof(uvec4) == sizeof(unsigned int) * 4, "No padding.");
	static_assert(sizeof(bvec4) == sizeof(bool        ) * 4, "No padding.");


	template <typename ElemType>
	struct Vector3
	{
		static_assert
		(
			std::is_arithmetic<ElemType>::value,
			"ElemType has to be arithmetic."
		);

		union
		{
			ElemType x;
			ElemType r;
			ElemType s;
			ElemType red;
			ElemType width;
		};

		union
		{
			ElemType g;
			ElemType y;
			ElemType t;
			ElemType green;
			ElemType height;
		};

		union
		{
			ElemType b;
			ElemType z;
			ElemType p;
			ElemType blue;
			ElemType depth;
		};

		Vector3 ()
		:
			x { ElemType { } },
			y { ElemType { } },
			z { ElemType { } }
		{
		}

		Vector3
		(
			const ElemType x,
			const ElemType y,
			const ElemType z
		):
			x { x },
			y { y },
			z { z }
		{
		}

		const ElemType& operator [] (const int index) const
		{
			assert(index >= 0 && index <= 2);

			if (index <= 0)
			{
				return x;
			}
			else if (index == 1)
			{
				return y;
			}
			else // index >= 2
			{
				return z;
			}
		}

		ElemType& operator [] (const int index)
		{
			assert(index >= 0 && index <= 2);

			if (index <= 0)
			{
				return x;
			}
			else if (index == 1)
			{
				return y;
			}
			else // index >= 2
			{
				return z;
			}
		}
	};

	using  vec3 = Vector3<float       >;
	using dvec3 = Vector3<double      >;
	using ivec3 = Vector3<int         >;
	using uvec3 = Vector3<unsigned int>;
	using bvec3 = Vector3<bool        >;

	static_assert(sizeof( vec3) == sizeof(float       ) * 3, "No padding.");
	static_assert(sizeof(dvec3) == sizeof(double      ) * 3, "No padding.");
	static_assert(sizeof(ivec3) == sizeof(int         ) * 3, "No padding.");
	static_assert(sizeof(uvec3) == sizeof(unsigned int) * 3, "No padding.");
	static_assert(sizeof(bvec3) == sizeof(bool        ) * 3, "No padding.");


	template <typename ElemType>
	struct Vector2
	{
		static_assert
		(
			std::is_arithmetic<ElemType>::value,
			"ElemType has to be arithmetic."
		);

		union
		{
			ElemType x;
			ElemType r;
			ElemType s;
			ElemType red;
			ElemType width;
		};

		union
		{
			ElemType g;
			ElemType y;
			ElemType t;
			ElemType green;
			ElemType height;
		};

		Vector2 ()
		:
			x { ElemType { } },
			y { ElemType { } }
		{
		}

		Vector2
		(
			const ElemType x,
			const ElemType y
		):
			x { x },
			y { y }
		{
		}

		const ElemType& operator [] (const int index) const
		{
			assert(index >= 0 && index <= 1);

			if (index <= 0)
			{
				return x;
			}
			else // index >= 1
			{
				return y;
			}
		}

		ElemType& operator [] (const int index)
		{
			assert(index >= 0 && index <= 1);

			if (index <= 0)
			{
				return x;
			}
			else // index >= 1
			{
				return y;
			}
		}
	};

	using  vec2 = Vector2<float       >;
	using dvec2 = Vector2<double      >;
	using ivec2 = Vector2<int         >;
	using uvec2 = Vector2<unsigned int>;
	using bvec2 = Vector2<bool        >;

	static_assert(sizeof( vec2) == sizeof(float       ) * 2, "No padding.");
	static_assert(sizeof(dvec2) == sizeof(double      ) * 2, "No padding.");
	static_assert(sizeof(ivec2) == sizeof(int         ) * 2, "No padding.");
	static_assert(sizeof(uvec2) == sizeof(unsigned int) * 2, "No padding.");
	static_assert(sizeof(bvec2) == sizeof(bool        ) * 2, "No padding.");
}

#endif
