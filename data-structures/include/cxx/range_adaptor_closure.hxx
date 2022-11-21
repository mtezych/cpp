
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Mateusz Zych
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


#ifndef CXX_RANGE_ADAPTOR_CLOSURE
#define CXX_RANGE_ADAPTOR_CLOSURE


#include <ranges>

#include <functional>

#include <type_traits>

#include <concepts>

#include <utility>


namespace cxx::detail
{
    template <template <class> class base,
                               class derived>
    //
    auto extract_derived_from_crtp_base (base<derived>*) -> derived*;


    template <                 class derived,
              template <class> class base>
    //
    inline constexpr auto inherits_from_crtp_base = requires (derived* ptr)
    {
        { cxx::detail::
          extract_derived_from_crtp_base<base>(ptr) } -> std::same_as<derived*>;
    };
}


namespace cxx::ranges
{
    // [ISO C++] P2387R3: Pipe support for user-defined range adaptors
    //
    // ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2387r3.html


    template <class derived_closure>
    //
    requires std::is_class_v<derived_closure> &&
             std::same_as   <derived_closure, std::remove_cv_t<derived_closure>>
    //
    class range_adaptor_closure_interface { };


    // [ISO C++] - Working Draft, C++23 Standard
    //
    //
    //  26.7.2 Range adaptor objects                      [range.adaptor.object]
    //
    //  (1) A range adaptor closure object is a unary function object
    //      that accepts a range argument. (...)
    //
    //  (2) Given an object t of type T, where
    //
    //      - t is a unary function object that accepts a range argument,
    //
    //      - T models derived_from<range_adaptor_closure<T>>,
    //
    //      - T has no other base classes of type range_adaptor_closure<U>
    //        for any other type U, and
    //
    //      - T does not model range, then
    //
    //      the implementation ensures that t is a range adaptor closure object.
    //
    //
    // ~ https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/n4917.pdf


    template <class type>
    concept maybe_range_adaptor_closure  =  detail::inherits_from_crtp_base<
    //
    std::remove_cvref_t<type>, cxx::ranges::range_adaptor_closure_interface>
    //
    &&                        !std::ranges::range<std::remove_cv_t<type>>;


    template <class range_adaptor_closure, typename viewable_range>
    //
    concept range_adaptor_closure_for =
    //
    cxx::ranges::maybe_range_adaptor_closure <range_adaptor_closure       > &&
    std::ranges::viewable_range              <              viewable_range> &&
    //
                        std::invocable       <range_adaptor_closure,
                                                            viewable_range> &&
    //
    std::ranges::view < std::invoke_result_t <range_adaptor_closure,
                                                            viewable_range> >;


    template <std::ranges::viewable_range               viewable_range,
              cxx::ranges::
              range_adaptor_closure_for<viewable_range> range_adaptor_closure>
    //
    [[nodiscard]]
    constexpr auto operator | (viewable_range&&        range,
                               range_adaptor_closure&& closure)
    //
    noexcept(std::is_nothrow_invocable_v<range_adaptor_closure, viewable_range>)
    {
        return std::invoke(std::forward<range_adaptor_closure>(closure),
                           std::forward<viewable_range       >(range  ));
    }


    template <cxx::ranges::maybe_range_adaptor_closure  left,
              cxx::ranges::maybe_range_adaptor_closure right>
    //
    class pipeline  // note: The pipeline class represents the result of
                   //        composing two range adaptor closure objects
    :             //         via the pipe operator.
        public
        cxx::ranges::range_adaptor_closure_interface<pipeline<left, right>>
    {
    private:
        [[no_unique_address]]  left  left_closure;
        [[no_unique_address]] right right_closure;

    public:
        template <typename left_type, typename right_type>
        //
        requires std::same_as<std::remove_cvref_t< left_type>,  left> &&
                 std::same_as<std::remove_cvref_t<right_type>, right>
        //
        constexpr explicit
        pipeline (left_type&& left_closure, right_type&& right_closure)
        //
        noexcept(std::is_nothrow_constructible_v< left,  left_type> &&
                 std::is_nothrow_constructible_v<right, right_type>)
        //
        requires std::constructible_from< left,  left_type> &&
                 std::constructible_from<right, right_type>
        :
             left_closure(std::forward< left_type>( left_closure)),
            right_closure(std::forward<right_type>(right_closure))
        {
            // note: The pipeline's constructor copy- or move-constructs
            //       left and right range adaptor closure objects.
        }

        template <std::ranges::viewable_range viewable_range>
        //
        requires
        cxx::ranges::range_adaptor_closure_for< left&, viewable_range> &&
        cxx::ranges::range_adaptor_closure_for<right&, viewable_range>
        //
        [[nodiscard]]
        constexpr auto operator () (viewable_range&& range) &
        //
        noexcept(std::is_nothrow_invocable_v< left&, viewable_range> &&
                 std::is_nothrow_invocable_v<right&,
                        std::invoke_result_t< left&, viewable_range>>)
        {
            return std::invoke(right_closure,
                   std::invoke( left_closure,
                               std::forward<viewable_range>(range)));
        }

        template <std::ranges::viewable_range viewable_range>
        //
        requires
        cxx::ranges::range_adaptor_closure_for<const  left&, viewable_range> &&
        cxx::ranges::range_adaptor_closure_for<const right&, viewable_range>
        //
        [[nodiscard]]
        constexpr auto operator () (viewable_range&& range) const &
        //
        noexcept(std::is_nothrow_invocable_v<const  left&, viewable_range> &&
                 std::is_nothrow_invocable_v<const right&,
                        std::invoke_result_t<const  left&, viewable_range>>)
        {
            return std::invoke(right_closure,
                   std::invoke( left_closure,
                               std::forward<viewable_range>(range)));
        }

        template <std::ranges::viewable_range viewable_range>
        //
        requires
        cxx::ranges::range_adaptor_closure_for< left&&, viewable_range> &&
        cxx::ranges::range_adaptor_closure_for<right&&, viewable_range>
        //
        [[nodiscard]]
        constexpr auto operator () (viewable_range&& range) &&
        //
        noexcept(std::is_nothrow_invocable_v< left&&, viewable_range> &&
                 std::is_nothrow_invocable_v<right&&,
                        std::invoke_result_t< left&&, viewable_range>>)
        {
            return std::invoke(std::move(right_closure),
                   std::invoke(std::move( left_closure),
                               std::forward<viewable_range>(range)));
        }

        template <std::ranges::viewable_range viewable_range>
        //
        requires
        cxx::ranges::range_adaptor_closure_for<const  left&&, viewable_range> &&
        cxx::ranges::range_adaptor_closure_for<const right&&, viewable_range>
        //
        [[nodiscard]]
        constexpr auto operator () (viewable_range&& range) const &&
        //
        noexcept(std::is_nothrow_invocable_v<const  left&&, viewable_range> &&
                 std::is_nothrow_invocable_v<const right&&,
                        std::invoke_result_t<const  left&&, viewable_range>>)
        {
            return std::invoke(std::move(right_closure),
                   std::invoke(std::move( left_closure),
                               std::forward<viewable_range>(range)));
        }
    };

    template <cxx::ranges::maybe_range_adaptor_closure  left,
              cxx::ranges::maybe_range_adaptor_closure right>
    //
    pipeline(left, right) -> pipeline<left, right>;
    //
    // note: The above deduction guide takes range adaptor closures by value,
    //
    //       and therefore deduces the left and the right types
    //       by performing the std::decay_t<> meta-function
    //       on the types of the arguments passed to the pipeline's constructor.
    //
    //       Since range adaptor closures are class types,
    //       this deduction guide effectively performs
    //       the std::remove_cvref_t<> meta-function on the argument types.


    template <cxx::ranges::maybe_range_adaptor_closure  left,
              cxx::ranges::maybe_range_adaptor_closure right>
    //
    [[nodiscard]]
    constexpr auto operator | (left&& left_closure, right&& right_closure)
    //
    noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t< left>,  left> &&
             std::is_nothrow_constructible_v<std::remove_cvref_t<right>, right>)
    //
    requires std::constructible_from<std::remove_cvref_t< left>,  left> &&
             std::constructible_from<std::remove_cvref_t<right>, right>
    {
        return pipeline
        {
            std::forward<left>(left_closure), std::forward<right>(right_closure)
        };
    }
}


#endif
