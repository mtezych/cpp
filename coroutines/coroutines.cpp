
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
			value_type value;

			promise_type ()
			:
				value { }
			{
				std::cout << "[ promise   ] constructor()" << std::endl;
			}
			~promise_type ()
			{
				std::cout << "[ promise   ] destructor()" << std::endl;
			}

			promise_type (promise_type&& promise)
			:
				value { std::move(promise.value) }
			{
				std::cout << "[ promise   ] move_constructor()" << std::endl;
			}
			promise_type (const promise_type& promise)
			:
				value { promise.value }
			{
				std::cout << "[ promise   ] copy_constructor()" << std::endl;
			}

			promise_type& operator = (promise_type&& promise)
			{
				std::cout << "[ promise   ] move_assignment()" << std::endl;

				value = std::move(promise.value);

				return *this;
			}
			promise_type& operator = (const promise_type& promise)
			{
				std::cout << "[ promise   ] copy_assignment()" << std::endl;

				value = promise.value;

				return *this;
			}

			auto get_return_object ()
			{
				std::cout << "[ promise   ] get_return_object() -> ";
				std::cout << "sync<value_type> { coroutine }" << std::endl;

				return sync<value_type>
				{
					std::experimental::coroutine_handle<promise_type>::from_promise(*this)
				};
			}

			auto initial_suspend ()
			{
				std::cout << "[ promise   ] initial_suspend()" << std::endl;

				return std::experimental::suspend_never { };
			}

			auto return_value (value_type&& value)
			{
				std::cout << "[ promise   ] return_value() <- ";
				std::cout << "value = " << value << std::endl;

				this->value = std::move(value);

				return std::experimental::suspend_never { };
			}

			auto return_value (value_type& value)
			{
				std::cout << "[ promise   ] return_value() <- ";
				std::cout << "value = " << value << std::endl;

				this->value = value;

				return std::experimental::suspend_never { };
			}

			auto final_suspend ()
			{
				std::cout << "[ promise   ] final_suspend()" << std::endl;

				return std::experimental::suspend_always { };
			}

			void unhandled_exception ()
			{
				std::cout << "[ promise   ] unhandled_exception()" << std::endl;

				std::terminate();
			}
		};

		std::experimental::coroutine_handle<promise_type> coroutine;

		explicit
		sync (std::experimental::coroutine_handle<promise_type> coroutine)
		:
			coroutine { coroutine }
		{
			std::cout << "[ sync      ] constructor() <- coroutine" << std::endl;
		}

		~sync ()
		{
			std::cout << "[ sync      ] destructor()" << std::endl;

			if (coroutine)
			{
				coroutine.destroy();
			}
		}

		sync (sync&& sync)
		:
			coroutine { sync.coroutine }
		{
			sync.coroutine = nullptr;

			std::cout << "[ sync      ] move_constructor()" << std::endl;
		}

		sync (const sync& sync) = delete;

		sync& operator = (sync&& sync)
		{
			std::cout << "[ sync      ] move_assignment()" << std::endl;

			if (coroutine)
			{
				coroutine.destroy();
			}

			coroutine = sync.coroutine;

			sync.coroutine = nullptr;

			return *this;
		}

		sync& operator = (const sync& sync) = delete;

		const value_type& get () const
		{
			std::cout << "[ sync      ] get() -> ";
			std::cout << "value = " << coroutine.promise().value << std::endl;

			return coroutine.promise().value;
		}

		value_type& get ()
		{
			std::cout << "[ sync      ] get() -> ";
			std::cout << "value = " << coroutine.promise().value << std::endl;

			return coroutine.promise().value;
		}
	};

	template <typename value_type>
	struct lazy
	{
		struct promise_type
		{
			value_type value;

			promise_type ()
			:
				value { }
			{
				std::cout << "[ promise   ] constructor()" << std::endl;
			}
			~promise_type ()
			{
				std::cout << "[ promise   ] destructor()" << std::endl;
			}

			promise_type (promise_type&& promise)
			:
				value { std::move(promise.value) }
			{
				std::cout << "[ promise   ] move_constructor()" << std::endl;
			}
			promise_type (const promise_type& promise)
			:
				value { promise.value }
			{
				std::cout << "[ promise   ] copy_constructor()" << std::endl;
			}

			promise_type& operator = (promise_type&& promise)
			{
				std::cout << "[ promise   ] move_assignment()" << std::endl;

				value = std::move(promise.value);

				return *this;
			}
			promise_type& operator = (const promise_type& promise)
			{
				std::cout << "[ promise   ] copy_assignment()" << std::endl;

				value = promise.value;

				return *this;
			}

			auto get_return_object ()
			{
				std::cout << "[ promise   ] get_return_object() -> ";
				std::cout << "lazy<value_type> { coroutine }" << std::endl;

				return lazy<value_type>
				{
					std::experimental::coroutine_handle<promise_type>::from_promise(*this)
				};
			}

			auto initial_suspend ()
			{
				std::cout << "[ promise   ] initial_suspend()" << std::endl;

				return std::experimental::suspend_always { };
			}

			auto return_value (value_type&& value)
			{
				std::cout << "[ promise   ] return_value() <- ";
				std::cout << "value = " << value << std::endl;

				this->value = std::move(value);

				return std::experimental::suspend_never { };
			}

			auto return_value (value_type& value)
			{
				std::cout << "[ promise   ] return_value() <- ";
				std::cout << "value = " << value << std::endl;

				this->value = value;

				return std::experimental::suspend_never { };
			}

			auto final_suspend ()
			{
				std::cout << "[ promise   ] final_suspend()" << std::endl;

				return std::experimental::suspend_always { };
			}

			void unhandled_exception ()
			{
				std::cout << "[ promise   ] unhandled_exception()" << std::endl;

				std::terminate();
			}
		};

		std::experimental::coroutine_handle<promise_type> coroutine;

		explicit
		lazy (std::experimental::coroutine_handle<promise_type> coroutine)
		:
			coroutine { coroutine }
		{
			std::cout << "[ lazy      ] constructor() <- coroutine" << std::endl;
		}

		~lazy ()
		{
			std::cout << "[ lazy      ] destructor()" << std::endl;

			if (coroutine)
			{
				coroutine.destroy();
			}
		}

		lazy (lazy&& lazy)
		:
			coroutine { lazy.coroutine }
		{
			lazy.coroutine = nullptr;

			std::cout << "[ lazy      ] move_constructor()" << std::endl;
		}

		lazy (const lazy& lazy) = delete;

		lazy& operator = (lazy&& lazy)
		{
			std::cout << "[ lazy      ] move_assignment()" << std::endl;

			if (coroutine)
			{
				coroutine.destroy();
			}

			coroutine = lazy.coroutine;

			lazy.coroutine = nullptr;

			return *this;
		}

		lazy& operator = (const lazy& lazy) = delete;

		const value_type& get () const
		{
			std::cout << "[ lazy      ] get() -> ..." << std::endl;

			if (!coroutine.done())
			{
				coroutine.resume();
			}

			std::cout << "[ lazy      ]          ... -> value = ";
			std::cout << coroutine.promise().value << std::endl;

			return coroutine.promise().value;
		}

		value_type& get ()
		{
			std::cout << "[ lazy      ] get() -> ..." << std::endl;

			if (!coroutine.done())
			{
				coroutine.resume();
			}

			std::cout << "[ lazy      ]          ... -> value = ";
			std::cout << coroutine.promise().value << std::endl;

			return coroutine.promise().value;
		}
	};

	struct empty
	{
		struct promise_type
		{
			promise_type ()
			{
				std::cout << "[ promise   ] constructor()" << std::endl;
			}
			~promise_type ()
			{
				std::cout << "[ promise   ] destructor()" << std::endl;
			}

			promise_type (promise_type&&)
			{
				std::cout << "[ promise   ] move_constructor()" << std::endl;
			}
			promise_type (const promise_type&)
			{
				std::cout << "[ promise   ] copy_constructor()" << std::endl;
			}

			promise_type& operator = (promise_type&&)
			{
				std::cout << "[ promise   ] move_assignment()" << std::endl;

				return *this;
			}
			promise_type& operator = (const promise_type&)
			{
				std::cout << "[ promise   ] copy_assignment()" << std::endl;

				return *this;
			}

			auto get_return_object ()
			{
				std::cout << "[ promise   ] get_return_object() -> ";
				std::cout << "empty { coroutine }" << std::endl;

				return empty
				{
					std::experimental::coroutine_handle<promise_type>::from_promise(*this)
				};
			}

			auto initial_suspend ()
			{
				std::cout << "[ promise   ] initial_suspend()" << std::endl;

				return std::experimental::suspend_never { };
			}

			auto return_void ()
			{
				std::cout << "[ promise   ] return_void()" << std::endl;

				return std::experimental::suspend_never { };
			}

			auto final_suspend ()
			{
				std::cout << "[ promise   ] final_suspend()" << std::endl;

				return std::experimental::suspend_always { };
			}

			void unhandled_exception ()
			{
				std::cout << "[ promise   ] unhandled_exception()" << std::endl;

				std::terminate();
			}
		};

		std::experimental::coroutine_handle<promise_type> coroutine;

		explicit
		empty (std::experimental::coroutine_handle<promise_type> coroutine)
		:
			coroutine { coroutine }
		{
			std::cout << "[ empty     ] constructor() <- coroutine" << std::endl;
		}

		~empty ()
		{
			std::cout << "[ empty     ] destructor()" << std::endl;

			if (coroutine)
			{
				coroutine.destroy();
			}
		}

		empty (empty&& empty)
		:
			coroutine { empty.coroutine }
		{
			empty.coroutine = nullptr;

			std::cout << "[ empty     ] move_constructor()" << std::endl;
		}

		empty (const empty& empty) = delete;

		empty& operator = (empty&& empty)
		{
			std::cout << "[ empty     ] move_assignment()" << std::endl;

			if (coroutine)
			{
				coroutine.destroy();
			}

			coroutine = empty.coroutine;

			empty.coroutine = nullptr;

			return *this;
		}

		empty& operator = (const empty& empty) = delete;
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

	auto MakeFoo () -> coroutines::sync<Foo>
	{
		std::cout << "[ foo       ] CreateFoo () -> sync<Foo>" << std::endl;

		co_return Foo { '#' };
	}

	auto CreateFoo () -> coroutines::lazy<Foo>
	{
		std::cout << "[ foo       ] GenerateFoo () -> lazy<Foo>" << std::endl;

		co_return Foo { '&' };
	}

	auto DoNothing () -> coroutines::empty
	{
		std::cout << "[ foo       ] DoNothing () -> empty" << std::endl;

		co_return;
	}
}

int main ()
{
	{
		auto sync = foo::MakeFoo();
		std::cout << "[ main      ] foo::MakeFoo() -> sync<Foo>" << std::endl;

		const auto& value = sync.get();
		std::cout << "[ main      ] sync.get() -> value = " << value << std::endl;
	}

	std::cout << std::endl;

	{
		auto lazy = foo::CreateFoo();
		std::cout << "[ main      ] foo::CreateFoo() -> lazy<Foo>" << std::endl;

		const auto& value = lazy.get();
		std::cout << "[ main      ] lazy.get() -> value = " << value << std::endl;
	}

	std::cout << std::endl;

	{
		auto empty = foo::DoNothing();
		std::cout << "[ main      ] foo::DoNothing() -> empty" << std::endl;
	}

	return 0;
}
