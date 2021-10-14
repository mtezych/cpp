
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


#ifndef CXX_RESULT
#define CXX_RESULT


#include <type_traits>

#include <concepts>

#include <utility>

#include <cassert>


namespace cxx
{
    // [WG21 P2098R0] - Proposing std::is_specialization_of
    //  - http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2098r0.pdf
    //
    // [WG21 P1985R2] - Universal Template Parameters
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p1985r1.pdf
    //
    template <typename type, template <typename...> class class_template>
    inline constexpr
    auto is_specialization_of = false;

    template <template <typename...> class class_template, typename ... types>
    inline constexpr
    auto is_specialization_of <class_template<types...>, class_template> = true;


    // note: Assume that static exceptions, proposed by Herb Sutter,
    //       have been accepted in a future version of ISO C++.
    //
    // [WG21 P0709R4] - Zero-overhead deterministic exceptions: Throwing values
    //  ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0709r4.pdf
    //
    // [WG21 D2170R0] - Feedback on designing the proposed std::error type
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2170r0.html


    // note: The std::error class is
    //       the C++ vocabulary type representing errors in C++ programs.
    //
    //       The std::error is concrete and type-erased type,
    //       capable of representing both std::error_codes and std::exceptions.
    //
    //       The std::error class stores two values { payload + domain }.
    //
    //        - payload : std::error_value -> stores actual value of the error
    //           ~ for std::error_code, payload holds integer value of the error
    //           ~ for std::exception, payload holds std::exception_ptr
    //
    //        - domain : std::error_domain -> defines how to interpret payload
    //           ~ discriminant used to distinguish domains
    //           ~ enables type-erasure
    //           ~ can be implemented as a pointer or a hash
    //
    //       An instance of std::error always represents an error,
    //       that is, std::error does not have a "valid" or "success" state.
    //
    class error
    {
    public:
        constexpr
        auto operator == (const error&) const noexcept -> bool = default;
    };


    // note: In C++ a function marked as throws behaves, from ABI point-of-view,
    //       as if its return type was a specialization of the std::result<>.
    //
    //       auto fn () throws   -> int;
    //       auto fn () noexcept -> cxx::result<int>;
    //
    //       In contrast to std::result::Result<T, E> from Rust,
    //       in C++ std::result<T> is parameterized only by the value_type,
    //       which enables uniform implementation of error-handling code
    //       in different C++ libraries, improving their composability.
    //
    //        ~ https://doc.rust-lang.org/stable/std/result
    //
    template <typename value_type>
    class result
    {
        static_assert(!
        std::is_same_v           <std::remove_cvref_t<value_type>, cxx:: error>);

        static_assert(!
        cxx::is_specialization_of<std::remove_cvref_t<value_type>, cxx::result>);

    private:
        union
        {
            value_type val;
            cxx::error err;
        };
        bool flag;

    public:
        explicit result (std::same_as<value_type> auto&& value)
        :
            flag { true                                 },
            val  { std::forward<decltype(value)>(value) }
        {
        }

        explicit result (cxx::error error)
        :
            flag { false },
            err  { error }
        {
        }

        explicit operator bool () const
        {
            return flag;
        }

        [[nodiscard]]
        auto value () const -> const value_type&
        {
            assert(flag);
            return val;
        }

        [[nodiscard]]
        auto error () const -> const cxx::error&
        {
            assert(!flag);
            return err;
        }
    };
}


#endif
