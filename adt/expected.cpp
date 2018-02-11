
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

#include <utility>
#include <cassert>

namespace adt
{
	template <typename value_type, typename error_type>
	struct expected
	{
		bool flag;
		union
		{
			value_type val;
			error_type err;
		};

		expected (value_type value)
		:
			flag { true             },
			val  { std::move(value) }
		{
		}

		expected (error_type error)
		:
			flag { false            },
			err  { std::move(error) }
		{
		}

		explicit operator bool() const
		{
			return flag;
		}

		const value_type& value () const
		{
			assert(flag);
			return val;
		}

		const error_type& error () const
		{
			assert(!flag);
			return err;
		}
	};
}

namespace assert
{
	template <typename value_type>
	void equal(const value_type& left, const value_type& right)
	{
		assert(left == right);
	}
}

namespace test
{
	void idiomatic_usage (bool flag)
	{
		struct Error
		{
			int value;

			bool operator == (const Error& error) const
			{
				return value == error.value;
			}
		};

		struct Foo
		{
			int value;

			static adt::expected<Foo, Error> Create (bool flag, int param)
			{
				if (flag)
				{
					return Foo { param };
				}
				else
				{
					return Error { param };
				}
			}

			bool operator == (const Foo& foo) const
			{
				return value == foo.value;
			}
		};

		const auto foo = Foo::Create(flag, 7);

		if (foo)
		{
			assert::equal(foo.value(), Foo { 7 });
		}
		else
		{
			assert::equal(foo.error(), Error { 7 });
		}
	}
}

int main ()
{
	test::idiomatic_usage(true);
	test::idiomatic_usage(false);

	return 0;
}
