
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <type_traits>


namespace adt
{
	//            +-------+   +-------+   +-------+
	//    head -->| next  |-->| next  |-->| next  |--> null
	//            |-------|   |-------|   |-------|
	//    null <--| prev  |<--| prev  |<--| prev  |<-- tail
	//            |-------|   |-------|   |-------|
	//            | value |   | value |   | value |
	//            +-------+   +-------+   +-------+

	template <typename value_type>
	class list
	{
		struct node
		{
			node*      next;
			node*      prev;
			value_type value;
		};

		node*  head;
		node*  tail;
		size_t length;

		template <bool constant>
		struct iterator
		{
		private:
			node* n;

			using element_type = typename
			std::conditional<constant, const value_type, value_type>::type;

		public:
			iterator (node* const n = nullptr)
			:
				n { n }
			{
			}

			element_type& operator * () const
			{
				assert(n != nullptr);
				return n->value;
			}

			element_type* operator -> () const
			{
				assert(n != nullptr);
				return &n->value;
			}

			iterator& operator ++ ()
			{
				assert(n != nullptr);
				n = n->next;
				return *this;
			}

			iterator& operator -- ()
			{
				assert(n != nullptr);
				n = n->prev;
				return *this;
			}

			bool operator == (const iterator& it) const
			{
				return this->n == it.n;
			}

			bool operator != (const iterator& it) const
			{
				return this->n != it.n;
			}
		};
		using mutable_iterator = iterator<false>;
		using   const_iterator = iterator<true >;

	public:
		list ()
		:
			head   { nullptr },
			tail   { nullptr },
			length { 0       }
		{
		}

		list (const std::initializer_list<value_type> init)
		:
			list { }
		{
			for (auto& elem : init)
			{
				push_back(elem);
			}
		}

		~list ()
		{
			clear();
		}

		list (const list& l)
		:
			list { }
		{
			for (const auto& elem : l)
			{
				push_back(elem);
			}
		}

		list& operator = (const list& l)
		{
			clear();

			for (const auto& elem : l)
			{
				push_back(elem);
			}

			return *this;
		}

		list (list&& l)
		:
			head   { l.head   },
			tail   { l.tail   },
			length { l.length }
		{
			l.head   = nullptr;
			l.tail   = nullptr;
			l.length = 0;
		}

		list& operator = (list&& l)
		{
			clear();

			head   = l.head;
			tail   = l.tail;
			length = l.length;

			l.head   = nullptr;
			l.tail   = nullptr;
			l.length = 0;

			return *this;
		}

		void push_back(const value_type& value)
		{
			const auto last = new node { nullptr, tail, value };

			if (length == 0)
			{
				assert(head == nullptr && tail == nullptr);
				head = last;
				tail = last;
			}
			else
			{
				assert(tail != nullptr && tail->next == nullptr);
				tail->next = last;
				tail       = last;
			}

			++length;
		}

		void push_front(const value_type& value)
		{
			const auto first = new node { head, nullptr, value };

			if (length == 0)
			{
				assert(head == nullptr && tail == nullptr);
				head = first;
				tail = first;
			}
			else
			{
				assert(head != nullptr && head->prev == nullptr);
				head->prev = first;
				head       = first;
			}

			++length;
		}

		void pop_front()
		{
			assert(length > 0 && head != nullptr);
			const auto first = head;

			if (length == 1)
			{
				head = nullptr;
				tail = nullptr;
			}
			else
			{
				head = first->next;
				head->prev = nullptr;
			}

			delete first;
			--length;
		}

		void pop_back()
		{
			assert(length > 0 && tail != nullptr);
			const auto last = tail;

			if (length == 1)
			{
				tail = nullptr;
				head = nullptr;
			}
			else
			{
				tail = last->prev;
				tail->next = nullptr;
			}

			delete last;
			--length;
		}

		const value_type& front() const
		{
			assert(head != nullptr);
			return head->value;
		}

		value_type& front()
		{
			assert(head != nullptr);
			return head->value;
		}

		const value_type& back() const
		{
			assert(tail != nullptr);
			return tail->value;
		}

		value_type& back()
		{
			assert(tail != nullptr);
			return tail->value;
		}

		size_t size() const
		{
			return length;
		}

		bool empty() const
		{
			return size() == 0;
		}

		void clear()
		{
			while (!empty())
			{
				pop_back();
			}
		}

		mutable_iterator begin()
		{
			return mutable_iterator { head };
		}

		const_iterator begin() const
		{
			return const_iterator { head };
		}

		const_iterator cbegin() const
		{
			return begin();
		}

		mutable_iterator end()
		{
			// --list.end() is invalid
			// this is not consistent with C++ iterator library
			return mutable_iterator { nullptr };
		}

		const_iterator end() const
		{
			// --list.end() is invalid
			// this is not consistent with C++ iterator library
			return const_iterator { nullptr };
		}

		const_iterator cend() const
		{
			return end();
		}
	};
}


namespace test
{
	void default_constructor()
	{
		auto list = adt::list<char> { };

		assert(list.empty());
		assert(list.size() == 0);
		assert(list. begin() == list. end());
		assert(list.cbegin() == list.cend());
	}

	void initializer_list_constructor()
	{
		const auto references = std::initializer_list<char> { '%', '9' };

		const auto list = adt::list<char> ( references );

		assert(!list.empty());
		assert(list.size() == references.size());
		assert(list.begin() != list.end());
	}

	void range_based_for_loop()
	{
		const auto references = std::initializer_list<char> { '%', '9' };

		const auto list = adt::list<char> ( references );

		auto reference = references.begin();
		for (const auto& e : list)
		{
			assert(e == *reference);
			++reference;
		}
	}

	void iterator_for_loop()
	{
		const auto references = std::initializer_list<char> { '%', '9' };

		const auto list = adt::list<char> ( references );

		auto reference = references.begin();
		for (auto elemIt = list.begin(); elemIt != list.end(); ++elemIt)
		{
			assert(*elemIt == *reference);
			++reference;
		}
	}

	void const_iterator_for_loop()
	{
		const auto references = std::initializer_list<char> { '$', '~', '@' };

		auto list = adt::list<char> ( references );

		auto reference = references.begin();
		for (auto elemIt = list.cbegin(); elemIt != list.cend(); ++elemIt)
		{
			assert(*elemIt == *reference);
			++reference;
		}

		list.clear();
	}

	void front()
	{
		const auto reference = '%';

		const auto list = adt::list<char> { reference, '*' };

		assert(list.front() == reference);
	}

	void back()
	{
		const auto reference = '%';

		const auto list = adt::list<char> { '&', '^', reference };

		assert(list.back() == reference);
	}

	void push_pop_front_back()
	{
		const auto assert_equal =
		[](const std::initializer_list<char> init, const adt::list<char>& list)
		{
			assert(init.size() == list.size());

			auto initIt = init.begin();
			auto listIt = list.begin();

			while (initIt != init.end() && listIt != list.end())
			{
				assert(*initIt == *listIt);

				++initIt;
				++listIt;
			}
		};

		auto list = adt::list<char> { '%', '9', '9' };

		list.push_back('4');
		assert_equal({      '%', '9', '9', '4'      }, list);
		list.push_front('&');
		assert_equal({ '&', '%', '9', '9', '4'      }, list);
		list.push_back('~');
		assert_equal({ '&', '%', '9', '9', '4', '~' }, list);
		list.pop_back();
		assert_equal({ '&', '%', '9', '9', '4'      }, list);
		list.pop_front();
		assert_equal({      '%', '9', '9', '4'      }, list);
		list.pop_back();
		assert_equal({      '%', '9', '9'           }, list);
		list.pop_back();
		assert_equal({      '%', '9'                }, list);
		list.pop_front();
		assert_equal({           '9'                }, list);
		list.push_back('7');
		assert_equal({           '9', '7'           }, list);
	}
}


int main()
{
	test::default_constructor();
	test::initializer_list_constructor();
	test::range_based_for_loop();
	test::iterator_for_loop();
	test::const_iterator_for_loop();
	test::front();
	test::back();
	test::push_pop_front_back();

	return 0;
}
