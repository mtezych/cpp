
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


#ifndef CXX_INDIRECT
#define CXX_INDIRECT


#include <cstdlib>

#include <utility>

#include <concepts>

#include <type_traits>


namespace cxx
{
    template <typename type>
    inline constexpr auto is_nothrow_equality_comparable = bool
    {
        std::equality_comparable<type>                         &&
        noexcept(std::declval<type>() == std::declval<type>()) &&
        noexcept(std::declval<type>() != std::declval<type>())
    };


    // [code::dive 2016] Sean Parent: Better Code
    //
    //  ~ https://www.youtube.com/watch?v=cK_kftBNgBc&t=2449s


    // [WG21 P0201] - An indirect value-type for C++
    //
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0201r0.pdf
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0201r1.pdf
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0201r2.pdf
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0201r3.pdf
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0201r4.pdf


    // [WG21 N4477] - Operator Dot
    //
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4173.pdf
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4477.pdf
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0416r1.pdf


    // [HOPL] - Bjarne Stroustrup: Thriving in a Crowded and Changing World
    //                             Proposals That Didn't Make C++17
    //
    //  ~ https://www.stroustrup.com/hopl20main-p5-p-bfc9cd4--final.pdf#page=105


    // note: The cxx::indirect<> represents an object,
    //       with dynamic storage duration, which can be accessed indirectly.
    //
    //       Ideally, C++ would support overloading of the operator dot,
    //       making implementation of smart references possible.
    //
    //       Then, the functionality provided by the cxx::indirect<>
    //       could be superseded by the smart reference std::unique_ref<>,
    //       which would, similarly to std::unique_ptr<>,
    //       own a heap-allocated object and manage its life-time.
    //
    //       Until then, the cxx::indirect<> can provide similar functionality,
    //       though it requires accessing value indirectly,
    //       through either the dereference operator or the arrow operator.
    //
    template <std::destructible value_type>
    class indirect
    {
    private:
        value_type* ptr;


    public:
        template <typename ... types>
        constexpr explicit indirect (types&& ... args)
        requires std::constructible_from<value_type, types&&...>
        :
            ptr { new value_type(std::forward<types>(args)...) }
        {
        }

        constexpr ~indirect () noexcept
        {
            delete ptr;
        }


        constexpr indirect (indirect&& other) noexcept
        :
            ptr { other.ptr }
        {
            other.ptr = nullptr;
        }

        constexpr auto operator = (indirect&& other) noexcept -> indirect&
        {
            delete ptr;

            this->ptr = other.ptr;
            other.ptr = nullptr;

            return *this;
        }


        constexpr indirect (const indirect& other)
        requires std::is_copy_constructible_v<value_type>
        :
            ptr { new value_type(*other) }
        {
        }

        constexpr auto operator = (const indirect& other)
        noexcept(std::is_nothrow_copy_assignable_v<value_type>) -> indirect&
        requires std::        is_copy_assignable_v<value_type>
        {
            // note: The cxx::indirect<> template class relies on
            //       the value_type to handle self-assignment correctly.
            //
            *ptr = *other;

            return *this;
        }


        [[nodiscard]]
        constexpr auto operator == (const indirect& other) const
        noexcept(cxx::is_nothrow_equality_comparable<value_type>) -> bool
        requires std::           equality_comparable<value_type>
        {
            return *this->ptr == *other.ptr;
        }


        [[nodiscard]]
        constexpr auto operator * () const &  noexcept -> const value_type&
        {
            return *ptr;
        }

        [[nodiscard]]
        constexpr auto operator * ()       &  noexcept ->       value_type&
        {
            return *ptr;
        }

        [[nodiscard]]
        constexpr auto operator * () const && noexcept -> const value_type&&
        {
            return std::move(*ptr);
        }

        [[nodiscard]]
        constexpr auto operator * ()       && noexcept ->       value_type&&
        {
            return std::move(*ptr);
        }


        [[nodiscard]]
        constexpr auto operator -> () const noexcept -> const value_type*
        {
            return ptr;
        }

        [[nodiscard]]
        constexpr auto operator -> ()       noexcept ->       value_type*
        {
            return ptr;
        }
    };
}


#endif
