
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2018, mtezych
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

// How C++ coroutines work
// ~ https://kirit.com/How%20C%2B%2B%20coroutines%20work

// Coroutine Theory
// ~ https://lewissbaker.github.io/2017/09/25/coroutine-theory

// Clang 5.0.0 Release Notes
// ~ https://releases.llvm.org/5.0.0/tools/clang/docs/ReleaseNotes.html

#include <experimental/coroutine>
#include <exception>
#include <iostream>
#include <memory>

namespace coroutines
{
	template <typename value_type>
	struct sync
	{
		struct promise_type
		{
			std::shared_ptr<value_type> shared_state;

			promise_type ()
			:
				shared_state { std::make_shared<value_type>() }
			{
				std::cout << "[ promise_type ] constructor()" << std::endl;
			}
			~promise_type ()
			{
				std::cout << "[ promise_type ] destructor()" << std::endl;
			}

			promise_type (promise_type&& promise)
			:
				shared_state { std::move(promise.shared_state) }
			{
				std::cout << "[ promise_type ] move_constructor()" << std::endl;
			}
			promise_type (const promise_type& promise)
			:
				shared_state { promise.shared_state }
			{
				std::cout << "[ promise_type ] copy_constructor()" << std::endl;
			}

			promise_type& operator = (promise_type&& promise)
			{
				std::cout << "[ promise_type ] move_assignment()" << std::endl;

				shared_state = std::move(promise.shared_state);

				return *this;
			}
			promise_type& operator = (const promise_type& promise)
			{
				std::cout << "[ promise_type ] copy_assignment()" << std::endl;

				shared_state = promise.shared_state;

				return *this;
			}

			auto get_return_object ()
			{
				std::cout << "[ promise_type ] get_return_object() -> sync<value_type> { shared_state }" << std::endl;

				return sync<value_type> { shared_state };
			}

			auto initial_suspend ()
			{
				std::cout << "[ promise_type ] initial_suspend()" << std::endl;

				return std::experimental::suspend_never { };
			}

			auto return_value (value_type&& value)
			{
				std::cout << "[ promise_type ] return_value() <- value = " << value << std::endl;

				*shared_state = std::move(value);

				return std::experimental::suspend_never { };
			}

			auto return_value (value_type& value)
			{
				std::cout << "[ promise_type ] return_value() <- value = " << value << std::endl;

				*shared_state = value;

				return std::experimental::suspend_never { };
			}

			auto final_suspend ()
			{
				std::cout << "[ promise_type ] final_suspend()" << std::endl;

				return std::experimental::suspend_never { };
			}

			void unhandled_exception ()
			{
				std::cout << "[ promise_type ] unhandled_exception()" << std::endl;

				std::terminate();
			}
		};

		std::shared_ptr<value_type> shared_state;

		explicit
		sync (std::shared_ptr<value_type> shared_state)
		:
			shared_state { std::move(shared_state) }
		{
			std::cout << "[ sync         ] constructor() <- shared_state" << std::endl;
		}
		~sync ()
		{
			std::cout << "[ sync         ] destructor()" << std::endl;
		}

		sync (sync&& sync)
		:
			shared_state { std::move(sync.shared_state) }
		{
			std::cout << "[ sync         ] move_constructor()" << std::endl;
		}
		sync (const sync& sync)
		:
			shared_state { sync.shared_state }
		{
			std::cout << "[ sync         ] copy_constructor()" << std::endl;
		}

		sync& operator = (sync&& sync)
		{
			std::cout << "[ sync         ] move_assignment()" << std::endl;

			shared_state = std::move(sync.shared_state);

			return *this;
		}
		sync& operator = (const sync& sync)
		{
			std::cout << "[ sync         ] copy_assignment()" << std::endl;

			shared_state = sync.shared_state;

			return *this;
		}

		const value_type& get () const
		{
			assert(shared_state != nullptr);

			std::cout << "[ sync         ] get() -> value = " << *shared_state << std::endl;

			return *shared_state;
		}

		value_type& get ()
		{
			assert(shared_state != nullptr);

			std::cout << "[ sync         ] get() -> value = " << *shared_state << std::endl;

			return *shared_state;
		}
	};
}

namespace foo
{
	struct Foo
	{
		char value;

		explicit
		Foo (const char value = '\0')
		:
			value { value }
		{
		}

		~Foo () = default;

		Foo (Foo&& foo) = default;
		Foo (const Foo& foo) = delete;

		Foo& operator = (Foo&& foo) = default;
		Foo& operator = (const Foo& foo) = delete;
	};

	std::ostream& operator << (std::ostream& os, const Foo& foo)
	{
		return os << "Foo { " << foo.value << " }";
	}

	auto CreateFoo () -> coroutines::sync<Foo>
	{
		std::cout << "[ foo          ] CreateFoo () -> sync<Foo>" << std::endl;

		co_return Foo { '#' };
	}
}

int main ()
{
	auto sync = foo::CreateFoo();
	std::cout << "[ main         ] foo::CreateFoo() -> sync<Foo>" << std::endl;

	const auto& value = sync.get();
	std::cout << "[ main         ] sync.get() -> value = " << value << std::endl;

	return 0;
}
