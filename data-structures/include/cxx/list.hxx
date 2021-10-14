
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2021, Mateusz Zych
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


#ifndef CXX_LIST
#define CXX_LIST


#include <cassert>

#include <cstddef>

#include <initializer_list>

#include <type_traits>


namespace cxx
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
    private:
        struct node
        {
            node*        next;
            node*        prev;
            value_type  value;
        };

        node*          head;
        node*          tail;
        std::size_t  length;

        template <bool constant>
        struct iterator
        {
        private:
            node* node_ptr;

            using element_type = std::conditional_t<constant, const value_type,
                                                                    value_type>;

        public:
            explicit constexpr iterator (node* const n = nullptr) noexcept
            :
                node_ptr { n }
            {
            }

            constexpr auto operator * () const noexcept -> element_type&
            {
                assert(node_ptr != nullptr);

                return node_ptr->value;
            }

            constexpr auto operator -> () const noexcept -> element_type*
            {
                assert(node_ptr != nullptr);

                return &node_ptr->value;
            }

            constexpr auto operator ++ () noexcept -> iterator&
            {
                assert(node_ptr != nullptr);

                node_ptr = node_ptr->next;

                return *this;
            }

            constexpr auto operator -- () noexcept -> iterator&
            {
                assert(node_ptr != nullptr);

                node_ptr = node_ptr->prev;

                return *this;
            }

            constexpr
            auto operator == (const iterator& it) const noexcept -> bool
            {
                return this->node_ptr == it.node_ptr;
            }

            constexpr
            auto operator != (const iterator& it) const noexcept -> bool
            {
                return this->node_ptr != it.node_ptr;
            }
        };

        using mutable_iterator = iterator<false>;
        using   const_iterator = iterator<true >;

    public:
        constexpr list () noexcept
        :
            head   { nullptr },
            tail   { nullptr },
            length {    0    }
        {
        }

        explicit list (const std::initializer_list<value_type> init_list)
        :
            list { }
        {
            for (auto& elem : init_list)
            {
                push_back(elem);
            }
        }

        ~list () noexcept
        {
            clear();
        }

        list (const list& other)
        :
            list { }
        {
            for (const auto& elem : other)
            {
                this->push_back(elem);
            }
        }

        auto operator = (const list& other) -> list&
        {
            auto  this_it = this->begin();
            auto other_it = other.begin();

            while ((this_it != this->end()) && (other_it != other.end()))
            {
                *this_it = *other_it;

                ++ this_it;
                ++other_it;
            }

            while (this->size() > other.size())
            {
                this->pop_back();
            }

            while (other_it != other.end())
            {
                this->push_back(*other_it);

                ++other_it;
            }

            return *this;
        }

        constexpr list (list&& other) noexcept
        :
            head   { other.head   },
            tail   { other.tail   },
            length { other.length }
        {
            other.head   = nullptr;
            other.tail   = nullptr;
            other.length = 0;
        }

        auto operator = (list&& other) noexcept -> list&
        {
            clear();

            head   = other.head;
            tail   = other.tail;
            length = other.length;

            other.head   = nullptr;
            other.tail   = nullptr;
            other.length = 0;

            return *this;
        }

        auto push_back (const value_type& value) -> void
        {
            const auto last = new node { nullptr, tail, value };

            if (length == 0)
            {
                assert((head == nullptr) && (tail == nullptr));
                head = last;
                tail = last;
            }
            else
            {
                assert((tail != nullptr) && (tail->next == nullptr));
                tail->next = last;
                tail       = last;
            }

            ++length;
        }

        auto push_front (const value_type& value) -> void
        {
            const auto first = new node { head, nullptr, value };

            if (length == 0)
            {
                assert((head == nullptr) && (tail == nullptr));
                head = first;
                tail = first;
            }
            else
            {
                assert((head != nullptr) && (head->prev == nullptr));
                head->prev = first;
                head       = first;
            }

            ++length;
        }

        auto pop_front () noexcept -> void
        {
            assert((length > 0) && (head != nullptr));

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

            --length;

            delete first;
        }

        auto pop_back () noexcept -> void
        {
            assert((length > 0) && (tail != nullptr));

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

            --length;

            delete last;
        }

        [[nodiscard]]
        constexpr auto front () const noexcept -> const value_type&
        {
            assert(head != nullptr);

            return head->value;
        }

        [[nodiscard]]
        constexpr auto front () noexcept -> value_type&
        {
            assert(head != nullptr);

            return head->value;
        }

        [[nodiscard]]
        constexpr auto back () const noexcept -> const value_type&
        {
            assert(tail != nullptr);

            return tail->value;
        }

        [[nodiscard]]
        constexpr auto back () noexcept -> value_type&
        {
            assert(tail != nullptr);

            return tail->value;
        }

        [[nodiscard]]
        constexpr auto size () const noexcept -> std::size_t
        {
            return length;
        }

        [[nodiscard]]
        constexpr auto empty () const noexcept -> bool
        {
            return length == 0;
        }

        auto clear () noexcept -> void
        {
            while (!empty())
            {
                pop_back();
            }
        }

        [[nodiscard]]
        constexpr auto begin () noexcept -> mutable_iterator
        {
            return mutable_iterator { head };
        }

        [[nodiscard]]
        constexpr auto begin () const noexcept -> const_iterator
        {
            return const_iterator { head };
        }

        [[nodiscard]]
        constexpr auto cbegin() const noexcept -> const_iterator
        {
            return begin();
        }

        [[nodiscard]]
        constexpr auto end () noexcept -> mutable_iterator
        {
            // bug: The expression --list.end() is not valid,
            //      which is not consistent with the C++ iterator library.
            //
            //      To fix this, without allocating additional memory,
            //      the list object itself could be interpreted as the end node.
            //
            return mutable_iterator { nullptr };
        }

        [[nodiscard]]
        constexpr auto end () const noexcept -> const_iterator
        {
            // bug: The expression --list.end() is not valid,
            //      which is not consistent with the C++ iterator library.
            //
            //      To fix this, without allocating additional memory,
            //      the list object itself could be interpreted as the end node.
            //
            return const_iterator { nullptr };
        }

        [[nodiscard]]
        constexpr auto cend () const noexcept -> const_iterator
        {
            return end();
        }
    };

    template <typename value_type>
    [[nodiscard]]
    constexpr auto operator == (const list<value_type>& left,
                                const list<value_type>& right) noexcept -> bool
    {
        if (left.size() != right.size())
        {
            return false;
        }
        else
        {
            auto  left_it = left .begin();
            auto right_it = right.begin();

            while (left_it != left.end())
            {
                if (*left_it != *right_it)
                {
                    return false;
                }

                ++ left_it;
                ++right_it;
            }

            return true;
        }
    }
}


#endif
