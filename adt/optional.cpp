
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

#include <type_traits>
#include <new>
#include <utility>
#include <cassert>

namespace adt
{
	template <typename value_type>
	class optional
	{
	private:
		bool empty;
		std::aligned_storage_t<sizeof(value_type), alignof(value_type)> storage;

	public:
		optional ()
		:
			empty { true }
		{
		}

		~optional ()
		{
			reset();
		}

		optional (const optional& optional)
		:
			empty { optional.empty }
		{
			if (optional)
			{
				new (&storage) value_type (optional.value());
			}
		}

		optional& operator = (const optional& optional)
		{
			reset();

			empty = optional.empty;

			if (optional)
			{
				new (&storage) value_type (optional.value());
			}

			return *this;
		}

		optional (optional&& optional)
		:
			empty { optional.empty }
		{
			if (optional)
			{
				new (&storage) value_type (std::move(optional.value()));
			}

			optional.reset();
		}

		optional& operator = (optional&& optional)
		{
			reset();

			empty = optional.empty;

			if (optional)
			{
				new (&storage) value_type (std::move(optional.value()));
			}

			optional.reset();

			return *this;
		}

		template <typename... Args>
		value_type& emplace (Args&&... args)
		{
			reset();

			new (&storage) value_type (std::forward<Args>(args)...);

			empty = false;

			return value();
		}

		void reset ()
		{
			if (has_value())
			{
				value().~value_type();

				empty = true;
			}
		}

		bool has_value() const
		{
			return !empty;
		}

		explicit operator bool() const
		{
			return has_value();
		}

		value_type& value ()
		{
			assert(has_value());

			return *reinterpret_cast<value_type*>(&storage);
		}

		const value_type& value () const
		{
			assert(has_value());

			return *reinterpret_cast<const value_type*>(&storage);
		}

		value_type& operator * ()
		{
			return value();
		}

		const value_type& operator * () const
		{
			return value();
		}

		value_type* operator -> ()
		{
			return &value();
		}

		const value_type* operator -> () const
		{
			return &value();
		}

		void swap (optional& optional)
		{
			if (has_value() && optional.has_value())
			{
				std::swap(value(), optional.value());
			}
			else if (optional.has_value())
			{
				new (&storage) value_type (std::move(optional.value()));
				optional.value().~value_type();
			}
			else if (has_value())
			{
				new (&optional.storage) value_type (std::move(value()));
				value().~value_type();
			}

			std::swap(empty, optional.empty);
		}
	};
}

namespace assert
{
	template <typename value_type>
	void empty (const adt::optional<value_type>& optional)
	{
		assert(!optional.has_value());
		assert(!optional);
	}

	template <typename value_type>
	void owns (const adt::optional<value_type>& optional, const value_type& value)
	{
		assert(optional.has_value());
		assert(optional);

		assert(optional.value() == value);
	}
}

namespace test
{
	void idiomatic_usage ()
	{
		struct Foo
		{
			int    a;
			char   b;
			double c;

			Foo (int a, char b, double c)
			:
				a { a },
				b { b },
				c { c }
			{
			}

			bool operator == (const Foo & foo) const
			{
				return (a == foo.a) && (b == foo.b) && (c == foo.c);
			}

			void Bar()
			{
			}
		};

		auto foo = adt::optional<Foo> { };
		assert::empty(foo);

		foo.emplace(4, '&', 8.0);
		assert::owns(foo, Foo { 4, '&', 8.0 });
		{
			if (foo)
			{
				foo->Bar();
			}
		}
		foo.reset();
		assert::empty(foo);
	}
}

int main ()
{
	test::idiomatic_usage();

	return 0;
}
