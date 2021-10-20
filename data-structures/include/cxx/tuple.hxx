
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


#ifndef CXX_TUPLE
#define CXX_TUPLE


#include <cstddef>

#include <utility>


namespace cxx
{
    namespace detail
    {
        template <std::size_t index, typename type>
        struct element
        {
            type value;

            constexpr
            auto operator == (const element&) const noexcept -> bool = default;
        };

        template <typename index_sequence, typename ... types>
        struct tuple_base;

        template <std::size_t ... indices, typename ... types>
        struct tuple_base <std::index_sequence<indices...>, types...>
        :
            element<indices, types>...
        {
            constexpr
            auto operator == (const tuple_base&) const noexcept -> bool = default;
        };
    }

    template <typename ... types>
    struct tuple
    :
        detail::tuple_base<std::index_sequence_for<types...>, types...>
    {
        static constexpr auto size = sizeof...(types);

        constexpr
        auto operator == (const tuple&) const noexcept -> bool = default;
    };

    template <typename ... types>
    tuple(types...) -> tuple<types...>;


    template <typename tuple>
    inline constexpr auto tuple_size = tuple::size;


    namespace detail
    {
        template <std::size_t index, typename type>
        auto select (const element<index, type>&) -> std::type_identity<type>;
    }

    template <std::size_t index, typename tuple> requires std::is_class_v<tuple>
    using tuple_element =
          typename decltype(detail::select<index>(std::declval<tuple>()))::type;


    namespace detail
    {
        template <std::size_t index, typename type>
        constexpr
        decltype(auto) slice_off (      element<index, type>&  element) noexcept
        {
            return std::forward<      type& >(element.value);
        }

        template <std::size_t index, typename type>
        constexpr
        decltype(auto) slice_off (const element<index, type>&  element) noexcept
        {
            return std::forward<const type& >(element.value);
        }

        template <std::size_t index, typename type>
        constexpr
        decltype(auto) slice_off (      element<index, type>&& element) noexcept
        {
            return std::forward<      type&&>(element.value);
        }

        template <std::size_t index, typename type>
        constexpr
        decltype(auto) slice_off (const element<index, type>&& element) noexcept
        {
            return std::forward<const type&&>(element.value);
        }
    }

    template <std::size_t index, typename tuple>
    constexpr
    decltype(auto) get (tuple&& t) noexcept
    {
        return detail::slice_off<index>(std::forward<tuple>(t));
    }
}


namespace std
{
    template <std::size_t index, typename ... types>
    struct tuple_element <index, cxx::tuple<types...>>
    {
        using type = cxx::tuple_element<index, cxx::tuple<types...>>;
    };

    template <typename ... types>
    struct tuple_size <cxx::tuple<types...>>
    :
        std::integral_constant<std::size_t, sizeof...(types)>
    {
    };
}


#endif
