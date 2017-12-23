
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
	template <typename value_type>
	class unique_ptr
	{
	private:
		value_type* ptr;

	public:
		unique_ptr ()
		:
			ptr { nullptr }
		{
		}

		unique_ptr (std::nullptr_t)
		:
			ptr { nullptr }
		{
		}

		explicit unique_ptr (value_type* const ptr)
		:
			ptr { ptr }
		{
		}

		~unique_ptr ()
		{
			delete ptr;
		}

		unique_ptr (const unique_ptr&) = delete;

		unique_ptr& operator = (const unique_ptr&) = delete;

		unique_ptr (unique_ptr&& uptr)
		:
			ptr { uptr.ptr }
		{
			uptr.ptr = nullptr;
		}

		unique_ptr& operator = (unique_ptr&& uptr)
		{
			delete ptr;
			ptr = uptr.ptr;
			uptr.ptr = nullptr;
			return *this;
		}

		unique_ptr& operator = (std::nullptr_t)
		{
			delete ptr;
			ptr = nullptr;
			return *this;
		}

		value_type& operator * () const
		{
			assert(ptr != nullptr);
			return *ptr;
		}

		value_type* operator -> () const
		{
			return ptr;
		}

		value_type* get () const
		{
			return ptr;
		}

		explicit operator bool () const
		{
			return ptr != nullptr;
		}
	
		value_type* release ()
		{
			const auto result = ptr;
			ptr = nullptr;
			return result;
		}

		void reset (value_type* const ptr = nullptr)
		{
			delete this->ptr;
			this->ptr = ptr;
		}
	};

	template <typename value_type>
	bool operator == (const unique_ptr<value_type>& uptr, std::nullptr_t)
	{
		return uptr.get() == nullptr;
	}

	template <typename value_type>
	bool operator != (const unique_ptr<value_type>& uptr, std::nullptr_t)
	{
		return uptr.get() != nullptr;
	}

	template <typename value_type>
	bool operator == (std::nullptr_t, const unique_ptr<value_type>& uptr)
	{
		return nullptr == uptr.get();
	}

	template <typename value_type>
	bool operator != (std::nullptr_t, const unique_ptr<value_type>& uptr)
	{
		return nullptr != uptr.get();
	}

	template <typename value_type, typename... args_types>
	unique_ptr<value_type> make_unique (args_types&&... args)
	{
		return unique_ptr<value_type> { new value_type { std::forward<args_types>(args)... } };
	}
}

namespace assert
{
	template <typename value_type>
	void empty (const adt::unique_ptr<value_type>& ptr)
	{
		assert(ptr.get() == nullptr);
		assert(ptr == nullptr);
		assert(!ptr);
	}

	template <typename value_type>
	void owns (const adt::unique_ptr<value_type>& ptr, const value_type& value)
	{
		assert(ptr.get() != nullptr);
		assert(ptr != nullptr);
		assert(ptr);

		assert(*ptr.get() == value);
		assert(*ptr == value);
	}

	void success()
	{
		static_assert(true);
	}

	void fail()
	{
		assert(false);
	}
}

#include <vector>

namespace test
{
	void default_constructor ()
	{
		const adt::unique_ptr<float> ptr { };

		assert::empty(ptr);
	}

	void converting_constructor ()
	{
		const adt::unique_ptr<char> ptr { nullptr };

		assert::empty(ptr);
	}

	void from_pointer_constructor ()
	{
		const adt::unique_ptr<char> ptr { new char { '#' } };

		assert::owns(ptr, '#');
	}

	void destructor ()
	{
		struct Foo
		{
		private:
			bool& isDestructorCalled;

		public:
			explicit Foo (bool& isDestructorCalled)
			:
				isDestructorCalled { isDestructorCalled }
			{
			}

			~Foo ()
			{
				isDestructorCalled = true;
			}

			bool operator == (const Foo& foo) const
			{
				return &isDestructorCalled == &foo.isDestructorCalled;
			}
		};

		auto isDestructorCalled = false;
		{
			const adt::unique_ptr<Foo> ptr { new Foo { isDestructorCalled } };

			assert::owns(ptr, Foo { isDestructorCalled });
		}
		assert(isDestructorCalled);
	}

	void move_constructor ()
	{
		const auto ptr = adt::unique_ptr<int> { new int { 8 } };

		assert::owns(ptr, 8);
	}

	void move_assignment ()
	{
		// Moving generally invalidates objects,
		// but this implementation should leave unique pointer in empty state.

		auto ptr1 = adt::unique_ptr<int> { new int { 7 } };
		auto ptr2 = adt::unique_ptr<int> { nullptr };
		assert::owns(ptr1, 7);
		assert::empty(ptr2);

		ptr2 = std::move(ptr1);
		assert::owns(ptr2, 7);
		assert::empty(ptr1);

		ptr1 = std::move(ptr2);
		assert::owns(ptr1, 7);
		assert::empty(ptr2);
	}

	void nullptr_assignment ()
	{
		auto ptr = adt::unique_ptr<int> { new int { 9 } };
		assert::owns(ptr, 9);

		ptr = nullptr;
		assert::empty(ptr);
	}

	void make_unique ()
	{
		struct Foo
		{
			int   i;
			float f;

			bool operator == (const Foo& foo) const
			{
				return (i == foo.i) && (f == foo.f);
			}
		};

		const auto ptr1 = adt::make_unique<Foo>(8, 1.0f);
		assert::owns(ptr1, Foo { 8, 1.0f });

		const auto ptr2 = adt::make_unique<Foo>();
		assert::owns(ptr2, Foo { });
	}

	void arrow_operator ()
	{
		struct Foo
		{
		private:
			mutable
			bool isBarCalled = false;
			bool isQuxCalled = false;

		public:
			const Foo& Bar () const
			{
				isBarCalled = true;
				return *this;
			}

			Foo& Qux ()
			{
				isQuxCalled = true;
				return *this;
			}

			bool operator == (const Foo& foo) const
			{
				return (isBarCalled == foo.isBarCalled) && (isQuxCalled == foo.isQuxCalled);
			}
		};

		const auto ptr1 = adt::make_unique<Foo>();
		assert::owns(ptr1, Foo { });

		ptr1->Qux();
		ptr1->Bar();
		assert::owns(ptr1, Foo { }.Qux().Bar());

		const auto ptr2 = adt::make_unique<const Foo>();
		assert::owns<const Foo>(ptr2, Foo { });

		ptr2->Bar();
		assert::owns<const Foo>(ptr2, Foo { }.Bar());
	}

	void bool_conversion_operator ()
	{
		auto ptr1 = adt::make_unique<int>(1);
		assert::owns(ptr1, 1);

		if (ptr1)
		{
			assert::success();
		}
		else
		{
			assert::fail();
		}

		auto ptr2 = adt::unique_ptr<int> { nullptr };
		assert::empty(ptr2);

		if (!ptr2)
		{
			assert::success();
		}
		else
		{
			assert::fail();
		}
	}

	void comparison_operators ()
	{
		auto ptr1 = adt::make_unique<int>(1);
		assert::owns(ptr1, 1);

		auto ptr2 = adt::unique_ptr<int> { nullptr };
		assert::empty(ptr2);

		const auto conditions = std::vector<bool>
		{
			ptr1 != nullptr, nullptr != ptr1,
			ptr2 == nullptr, nullptr == ptr2,
		};

		for (auto condition : conditions)
		{
			if (condition)
			{
				assert::success();
			}
			else
			{
				assert::fail();
			}
		}
	}

	void release ()
	{
		auto ptr = adt::make_unique<int>(4);
		assert::owns(ptr, 4);

		delete ptr.release();
		assert::empty(ptr);
	}

	void reset_nullptr ()
	{
		auto ptr = adt::make_unique<int>(2);
		assert::owns(ptr, 2);

		ptr.reset();
		assert::empty(ptr);
	}

	void reset ()
	{
		auto ptr = adt::make_unique<int>(1);
		assert::owns(ptr, 1);

		ptr.reset(new int { 11 });
		assert::owns(ptr, 11);
	}
}

int main ()
{
	test::default_constructor();
	test::converting_constructor();
	test::from_pointer_constructor();
	test::destructor();
	test::move_constructor();
	test::move_assignment();
	test::nullptr_assignment();
	test::make_unique();
	test::arrow_operator();
	test::bool_conversion_operator();
	test::comparison_operators();
	test::release();
	test::reset_nullptr();
	test::reset();

	return 0;
}
