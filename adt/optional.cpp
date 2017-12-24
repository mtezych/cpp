
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
	struct nullopt_t
	{
	};

	constexpr nullopt_t nullopt { };

	template <typename value_type>
	class optional
	{
	private:
		bool initialized;
		std::aligned_storage_t<sizeof(value_type), alignof(value_type)> storage;

	public:
		optional ()
		:
			initialized { false }
		{
		}

		optional (const nullopt_t)
		:
			initialized { false }
		{
		}

		~optional ()
		{
			reset();
		}

		optional (const optional& optional)
		:
			initialized { optional.initialized }
		{
			if (optional)
			{
				new (&storage) value_type (optional.value());
			}
		}

		optional& operator = (const optional& optional)
		{
			reset();

			initialized = optional.initialized;

			if (optional)
			{
				new (&storage) value_type (optional.value());
			}

			return *this;
		}

		optional (optional&& optional)
		:
			initialized { optional.initialized }
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

			initialized = optional.initialized;

			if (optional)
			{
				new (&storage) value_type (std::move(optional.value()));
			}

			optional.reset();

			return *this;
		}

		optional& operator = (const nullopt_t)
		{
			reset();

			return *this;
		}

		template <typename... Args>
		value_type& emplace (Args&&... args)
		{
			reset();

			new (&storage) value_type (std::forward<Args>(args)...);

			initialized = true;

			return value();
		}

		void reset ()
		{
			if (has_value())
			{
				value().~value_type();

				initialized = false;
			}
		}

		bool has_value() const
		{
			return initialized;
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

			std::swap(initialized, optional.initialized);
		}
	};

	template <typename value_type>
	bool operator == (const optional<value_type>& optional, const nullopt_t)
	{
		return !optional.has_value();
	}

	template <typename value_type>
	bool operator != (const optional<value_type>& optional, const nullopt_t)
	{
		return optional.has_value();
	}

	template <typename value_type>
	bool operator == (const nullopt_t, const optional<value_type>& optional)
	{
		return !optional.has_value();
	}

	template <typename value_type>
	bool operator != (const std::nullptr_t, const optional<value_type>& optional)
	{
		return optional.has_value();
	}

	template <typename value_type, typename... args_types>
	optional<value_type> make_optional (args_types&&... args)
	{
		auto opt = optional<value_type> { };

		opt.emplace(std::forward<args_types>(args)...);

		return opt;
	}
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

		bool operator == (const Foo& foo) const
		{
			return (a == foo.a) && (b == foo.b) && (c == foo.c);
		}

		void Bar ()
		{
		}
	};

	void idiomatic_usage ()
	{
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

	void null_optional_construction ()
	{
		const adt::optional<int> optional = adt::nullopt;
		assert::empty(optional);
	}

	void null_optional_assignment ()
	{
		auto optional = adt::optional<char> { };
		assert::empty(optional);

		optional.emplace('&');
		assert::owns(optional, '&');

		optional = adt::nullopt;
		assert::empty(optional);
	}

	void make_optional ()
	{
		const auto optional = adt::make_optional<Foo>(11, '&', 111.0);
		assert::owns(optional, Foo { 11, '&', 111.0 });
	}
}

int main ()
{
	test::idiomatic_usage();
	test::null_optional_construction();
	test::null_optional_assignment();
	test::make_optional();

	return 0;
}
