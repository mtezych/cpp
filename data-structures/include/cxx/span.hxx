
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


#ifndef CXX_SPAN
#define CXX_SPAN


#include <type_traits>

#include <utility>

#include <cstddef>

#include <iterator>


namespace cxx
{
    namespace detail
    {
        template <typename container>
        using elements_type = std::remove_pointer_t<
                              decltype(std::declval<container>().data())>;

        template <typename container, typename element_type>
        struct are_container_elements_convertible_to
        {
            static constexpr auto value =
                  std::is_convertible_v<elements_type<container>, element_type>;
        };
    }

    template <typename elem_type>
    class span
    {
    public:
        using    element_type = elem_type;
        using      value_type = std::remove_cv_t<elem_type>;

        using       size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using         pointer =       elem_type*;
        using   const_pointer = const elem_type*;

        using       reference =       elem_type&;
        using const_reference = const elem_type&;

    private:
        element_type* buffer;
        size_type     length;

    public:
        class iterator
        {
        private:
            element_type* ptr;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = span::value_type;
            using difference_type   = span::difference_type;
            using pointer           = span::pointer;
            using reference         = span::reference;

            explicit constexpr iterator (element_type* p = nullptr) noexcept
            :
                ptr { p }
            {
            }

            constexpr auto operator * () const noexcept -> element_type&
            {
                return *ptr;
            }

            constexpr auto operator -> () const noexcept -> element_type*
            {
                return ptr;
            }

            constexpr auto operator ++ () noexcept -> iterator&
            {
                ++ptr;
                return *this;
            }

            constexpr auto operator ++ (int) noexcept -> iterator
            {
                const auto it = *this;
                ++*this;
                return it;
            }

            constexpr auto operator -- () noexcept -> iterator&
            {
                --ptr;
                return *this;
            }

            constexpr auto operator -- (int) noexcept -> iterator
            {
                const auto it = *this;
                --*this;
                return it;
            }

            constexpr
            auto operator += (const difference_type n) noexcept -> iterator&
            {
                ptr += n;
                return *this;
            }

            constexpr
            auto operator -= (const difference_type n) noexcept -> iterator&
            {
                ptr -= n;
                return *this;
            }

            constexpr
            auto operator + (const difference_type n) const noexcept -> iterator
            {
                auto it = *this;
                it += n;
                return it;
            }

            constexpr
            auto operator - (const difference_type n) const noexcept -> iterator
            {
                auto it = *this;
                it -= n;
                return it;
            }

            friend constexpr
            auto operator + (const difference_type n,
                             const iterator&       it) noexcept -> iterator
            {
                return it + n;
            }

            constexpr auto operator - (const iterator& other) const noexcept
            -> difference_type
            {
                return this->ptr - other.ptr;
            }

            constexpr auto operator [] (const difference_type index) const
            noexcept -> element_type&
            {
                return ptr[index];
            }

            constexpr
            auto operator == (const iterator& other) const noexcept -> bool
            {
                return this->ptr == other.ptr;
            }

            constexpr
            auto operator != (const iterator& other) const noexcept -> bool
            {
                return !(*this == other);
            }

            constexpr
            auto operator < (const iterator& other) const noexcept -> bool
            {
                return this->ptr < other.ptr;
            }

            constexpr
            auto operator > (const iterator& other) const noexcept -> bool
            {
                return other < *this;
            }

            constexpr
            auto operator <= (const iterator& other) const noexcept -> bool
            {
                return !(*this > other);
            }

            constexpr
            auto operator >= (const iterator& other) const noexcept -> bool
            {
                return !(*this < other);
            }
        };

        using reverse_iterator = std::reverse_iterator<iterator>;

        constexpr span () noexcept
        :
            buffer { nullptr },
            length { 0       }
        {
        }

        constexpr span (elem_type* ptr, size_type count) noexcept
        :
            buffer { ptr   },
            length { count }
        {
        }

        constexpr span (elem_type* first, elem_type* last) noexcept
        :
            buffer { first },
            length { static_cast<size_type>(last - first) }
        {
        }

        template <typename container,
                  std::enable_if_t<detail::are_container_elements_convertible_to
                                           <container&, element_type>::value,
                                   int> = 0>
        explicit span (container& cont) noexcept
        :
            buffer { cont.data() },
            length { cont.size() }
        {
        }

        template <typename container,
                  std::enable_if_t<detail::are_container_elements_convertible_to
                                      <const container&, element_type>::value &&
                                   std::is_const<element_type>::value,
                                   int> = 0>
        explicit span (const container& cont) noexcept
        :
            buffer { cont.data() },
            length { cont.size() }
        {
        }

        [[nodiscard]]
        constexpr auto size () const noexcept -> size_type
        {
            return length;
        }

        [[nodiscard]]
        constexpr auto size_bytes () const noexcept -> size_type
        {
            return sizeof(element_type) * length;
        }

        [[nodiscard]]
        constexpr auto empty () const noexcept -> bool
        {
            return size() == 0;
        }

        constexpr auto data () const noexcept -> element_type*
        {
            return buffer;
        }

        constexpr
        auto operator [] (const size_type index) const noexcept -> element_type&
        {
            return buffer[index];
        }

        constexpr auto front () const noexcept -> element_type&
        {
            return buffer[0];
        }

        constexpr auto back () const noexcept -> element_type&
        {
            return buffer[length - 1];
        }

        constexpr auto begin () const noexcept -> iterator
        {
            return iterator { buffer };
        }

        constexpr auto end() const noexcept -> iterator
        {
            return iterator { buffer + length };
        }

        constexpr auto rbegin () const noexcept -> reverse_iterator
        {
            return reverse_iterator { end() };
        }

        constexpr auto rend () const noexcept -> reverse_iterator
        {
            return reverse_iterator { begin() };
        }
    };
}


#endif
