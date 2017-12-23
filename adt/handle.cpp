
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
	class handle
	{
	private:
		bool created;
		std::aligned_storage_t<sizeof(value_type), alignof(value_type)> storage;

	public:
		handle ()
		:
			created { false }
		{
		}

		~handle()
		{
			destroy();
		}

		handle (const handle& handle)
		:
			created { handle.created }
		{
			if (created)
			{
				new (&storage) value_type (*handle.get());
			}
		}

		handle& operator = (const handle& handle)
		{
			destroy();

			created = handle.created;

			if (created)
			{
				new (&storage) value_type (*handle.get());
			}

			return *this;
		}

		handle (handle&& handle)
		:
			created { handle.created }
		{
			if (created)
			{
				new (&storage) value_type (std::move(*handle.get()));
			}

			handle.destroy();
		}

		handle& operator = (handle&& handle)
		{
			destroy();

			created = handle.created;

			if (created)
			{
				new (&storage) value_type (std::move(*handle.get()));
			}

			handle.destroy();

			return *this;
		}

		template <typename... Args>
		void create (Args&&... args)
		{
			destroy();

			new (&storage) value_type (std::forward<Args>(args)...);

			created = true;
		}

		void destroy ()
		{
			if (created)
			{
				created = false;

				get()->~value_type();
			}
		}

		explicit operator bool() const
		{
			return created;
		}

		value_type* get ()
		{
			if (created)
			{
				return reinterpret_cast<value_type*>(&storage);
			}
			else
			{
				return nullptr;
			}
		}

		const value_type* get () const
		{
			if (created)
			{
				return reinterpret_cast<const value_type*>(&storage);
			}
			else
			{
				return nullptr;
			}
		}

		value_type* operator -> ()
		{
			assert(created);

			return get();
		}

		const value_type* operator -> () const
		{
			assert(created);

			return get();
		}

		void swap (handle& handle)
		{
			if (created && handle.created)
			{
				std::swap(*get(), *handle.get());
			}
			else if (handle.created)
			{
				new (&storage) value_type (std::move(*handle.get()));
				handle.get()->~value_type();
			}
			else if (created)
			{
				new (&handle.storage) value_type (std::move(*get()));
				get()->~value_type();
			}

			std::swap(created, handle.created);
		}
	};
}

namespace assert
{
	template <typename value_type>
	void empty (const adt::handle<value_type>& handle)
	{
		assert(handle.get() == nullptr);
		assert(!handle);
	}

	template <typename value_type>
	void owns (const adt::handle<value_type>& handle, const value_type& value)
	{
		assert(handle.get() != nullptr);
		assert(handle);

		assert(*handle.get() == value);
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

		adt::handle<Foo> foo;
		assert::empty(foo);

		foo.create(4, '&', 8.0);
		assert::owns(foo, Foo { 4, '&', 8.0 });
		{
			if (foo)
			{
				foo->Bar();
			}
		}
		foo.destroy();
		assert::empty(foo);
	}
}

int main ()
{
	test::idiomatic_usage();

	return 0;
}
