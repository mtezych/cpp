
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


#ifndef CXX_CHUNK_EVENLY
#define CXX_CHUNK_EVENLY


#include <cxx/contracts.hxx>

#include <cxx/integer_like.hxx>

#include <cxx/losslessly_convertible_to.hxx>

#include <cxx/ranges.hxx>

#include <cxx/range_adaptor_closure.hxx>

#include <type_traits>

#include <concepts>

#include <utility>

#include <ranges>


namespace cxx::detail
{
    template <cxx::maybe_integer_like integer_like>
    constexpr
    auto decrement_if (integer_like& value,
                       const bool    condition) noexcept -> void
    {
        // note: Unconditionally subtracting 0 or 1,
        //       depending on the value of the condition,
        //
        //       avoids generating an assembly code with a branch instruction.
        //
        value -= integer_like { condition };
    }


    template <cxx::maybe_integer_like integer_like>
    constexpr
    auto value_plus_one_or_value (const integer_like value,
                                  const bool         condition)
    noexcept                                                   -> integer_like
    {
        // note: This function returns value + 1 if the condition is true,
        //       otherwise it returns value.
        //
        return value + integer_like { condition };
    }


    template <std::ranges::range left, std::ranges::range right>
    inline constexpr
    auto same_range_diff = std::same_as<std::ranges::range_difference_t<left>,
                                        std::ranges::range_difference_t<right>>;
}


namespace cxx
{
    template <std::ranges::view view>
    //
    requires  cxx::ranges::sized_random_access_range<view>
    //
    class chunk_evenly_view
    :
        public std::ranges::view_interface<chunk_evenly_view<view>>
    {
        //       chunk_count
        //  <------------------->
        //  +---+---+---|---+---+ ^
        //  | 0 | 3 | 6 | 9 |   | |
        //  +---+---+---|---+---+ | quotient
        //  | 1 | 4 | 7 |...|   | |
        //  +---+---+---|---+---+ V
        //  | 2 | 5 | 8 | <---- border
        //  +---+---+---|
        //  <----------->
        //    remainder

        // [D Programming Language] - std.range.EvenChunks/evenChunks
        //
        // ~ https://dlang.org/library/std/range/even_chunks.html
        // ~ https://github.com/dlang/phobos/blob/master/std/range/package.d#L8526

    private:
        using range_diff = std::ranges::range_difference_t<view>;

        [[no_unique_address]]
        view         range;         // note: underlying (adapted) range
        range_diff   chunk_count;

        template <std::random_access_iterator range_iter>
        class iterator
        {
        private:
            [[no_unique_address]]
            range_iter  chunk_begin;
            range_diff  chunk_index;
            range_diff  chunk_size;
            range_diff  remainder;

            struct chunk_info
            {
                range_iter  chunk_begin;
                range_diff  chunk_index;
            };

            template <bool chunk_surely_after_border>
            //
            constexpr iterator (const  range_diff  range_size,
                                const  range_diff  chunk_count,
                                       chunk_info  chunk_info,
                                std::bool_constant<chunk_surely_after_border>)
            //
            noexcept(std::is_nothrow_move_constructible_v<range_iter>)
            :
                chunk_begin { std::move(chunk_info.chunk_begin) },
                chunk_index {           chunk_info.chunk_index  }
            {
                const auto
                quotient  = range_size / chunk_count;
                remainder = range_size % chunk_count;

                const auto chunk_before_border = !chunk_surely_after_border &&
                                                 (chunk_index < remainder);
                chunk_size =
                detail::value_plus_one_or_value(quotient, chunk_before_border);
            }

            friend class chunk_evenly_view<view>;

        public:
            // todo: The implementation of chunk_evenly_view::iterator
            //       is incomplete, because is does not model
            //       std::random_access_iterator concept, even though it could.
            //
            using iterator_concept  = std::bidirectional_iterator_tag;
            using iterator_category = std::        input_iterator_tag;
            using value_type        = std::ranges::subrange<range_iter,
                                                            range_iter>;
            using difference_type   = range_diff;

            constexpr iterator () = default;

        private:
            constexpr auto chunk_end () const
            noexcept(
            noexcept(  chunk_begin + chunk_size  )) -> range_iter
            {
                return chunk_begin + chunk_size;
            }

        public:
            constexpr auto operator * () const
            noexcept(
            noexcept(  std::ranges::subrange { chunk_begin,
                                               chunk_end() }  )) -> value_type
            {
                return std::ranges::subrange { chunk_begin,
                                               chunk_end() };
            }

            constexpr auto operator ++ ()
            noexcept(
            noexcept(chunk_begin += chunk_size)) -> iterator&
            {
                chunk_begin += chunk_size;

                ++chunk_index;

                const auto is_crossing_border = chunk_index == remainder;

                detail::decrement_if(chunk_size, is_crossing_border);

                return *this;
            }

            constexpr auto operator -- ()
            noexcept(
            noexcept(chunk_begin -= chunk_size)) -> iterator&
            {
                const auto is_crossing_border = chunk_index == remainder;

                const auto prev_chunk_size    =
                detail::value_plus_one_or_value(chunk_size, is_crossing_border);

                chunk_begin -= prev_chunk_size;
                chunk_size   = prev_chunk_size;

                --chunk_index;

                return *this;
            }

            constexpr auto operator ++ (int)
            //
            noexcept(std::is_nothrow_copy_constructible_v<iterator> &&
                     std::is_nothrow_move_constructible_v<iterator> &&
            noexcept(++*this))                                      -> iterator
            {
                // note: It is guaranteed that the range_iter is std::copyable,
                //       because it models std::random_access_iterator.
                //
                auto old = std::as_const(*this);
                ++*this;
                return old;
            }

            constexpr auto operator -- (int)
            //
            noexcept(std::is_nothrow_copy_constructible_v<iterator> &&
                     std::is_nothrow_move_constructible_v<iterator> &&
            noexcept(--*this))                                      -> iterator
            {
                auto old = std::as_const(*this);
                --*this;
                return old;
            }

            friend
            constexpr auto operator == (const iterator& left,
                                        const iterator& right) noexcept -> bool
            {
                return left.chunk_index == right.chunk_index;
            }

            friend
            constexpr auto operator != (const iterator& left,
                                        const iterator& right) noexcept -> bool
            {
                return left.chunk_index != right.chunk_index;
            }
        };

        using mutable_range_iter = std::ranges::iterator_t<      view>;
        using   const_range_iter = std::ranges::iterator_t<const view>;

    public:
        constexpr explicit
        chunk_evenly_view (view range, const range_diff chunk_count)
        //
        noexcept(std::is_nothrow_move_constructible_v<view>)
        :
            range       { std::move(range) },
            chunk_count { chunk_count      }
        {
            cxx_expects(chunk_count > 0);
        }

        [[nodiscard]]
        constexpr auto begin ()
        //
        noexcept(std::      is_nothrow_move_constructible_v<mutable_range_iter> &&
        noexcept(std::ranges::begin(range))                                     &&
        noexcept(cxx::ranges::ssize(range)))    -> iterator<mutable_range_iter>
        //
        requires (!cxx::ranges::simple_range<view>)
        {
            constexpr auto chunk_surely_after_border = std::false_type { };
            return
            {
                cxx::ranges::ssize(range), chunk_count,
                {
                    .chunk_begin = std::ranges::begin(range),
                    .chunk_index = 0,
                },
                chunk_surely_after_border,
            };
        }

        [[nodiscard]]
        constexpr auto begin () const
        //
        noexcept(std::      is_nothrow_move_constructible_v<const_range_iter> &&
        noexcept(std::ranges::begin(range))                                   &&
        noexcept(cxx::ranges::ssize(range)))    -> iterator<const_range_iter>
        //
        requires cxx::ranges::sized_random_access_range<const view>
        {
            static_assert(detail::same_range_diff<view, const view>);

            constexpr auto chunk_surely_after_border = std::false_type { };
            return
            {
                cxx::ranges::ssize(range), chunk_count,
                {
                    .chunk_begin = std::ranges::begin(range),
                    .chunk_index = 0,
                },
                chunk_surely_after_border,
            };
        }

        [[nodiscard]]
        constexpr auto end ()
        //
        noexcept(std::      is_nothrow_move_constructible_v<mutable_range_iter> &&
        noexcept(cxx::ranges::end_iter(range))                                  &&
        noexcept(cxx::ranges::   ssize(range))) -> iterator<mutable_range_iter>
        //
        requires (!cxx::ranges::simple_range<view>)
        {
            constexpr auto chunk_surely_after_border = std::true_type { };
            return
            {
                cxx::ranges::ssize(range), chunk_count,
                {
                    .chunk_begin = cxx::ranges::end_iter(range),
                    .chunk_index = chunk_count,
                },
                chunk_surely_after_border,
            };
        }

        [[nodiscard]]
        constexpr auto end () const
        //
        noexcept(std::      is_nothrow_move_constructible_v<const_range_iter> &&
        noexcept(cxx::ranges::end_iter(range))                                &&
        noexcept(cxx::ranges::   ssize(range))) -> iterator<const_range_iter>
        //
        requires cxx::ranges::sized_random_access_range<const view>
        {
            static_assert(detail::same_range_diff<view, const view>);

            constexpr auto chunk_surely_after_border = std::true_type { };
            return
            {
                cxx::ranges::ssize(range), chunk_count,
                {
                    .chunk_begin = cxx::ranges::end_iter(range),
                    .chunk_index = chunk_count,
                },
                chunk_surely_after_border,
            };
        }

        [[nodiscard]]
        constexpr auto size () const noexcept -> range_diff
        {
            return chunk_count;
        }

        [[nodiscard]]
        constexpr auto size ()       noexcept -> range_diff
        {
            return chunk_count;
        }
    };
}


template <typename view>
inline constexpr
auto std::ranges::enable_borrowed_range<cxx::chunk_evenly_view<view>> = bool
{
     std::ranges::enable_borrowed_range<view>
};


namespace cxx
{
    template <cxx::maybe_integer_like integer_like>
    //
    class chunk_evenly_closure
    :
        public cxx::ranges::range_adaptor_closure_interface<
                                       chunk_evenly_closure<integer_like>>
    {
    private:
        template <std::ranges::range range>
        //
        using range_diff = std::ranges::range_difference_t<range>;

        integer_like chunk_count;

    public:
        constexpr explicit
        chunk_evenly_closure (const integer_like chunk_count) noexcept
        :
            chunk_count { chunk_count }
        {
            cxx_expects(chunk_count > 0);
        }

        template <std::ranges::viewable_range viewable_range>
        //
        requires  cxx::ranges::sized_random_access_range<viewable_range>
        //
        [[nodiscard]]
        constexpr auto operator () (viewable_range&& range) const
        //
        noexcept(
        noexcept(  chunk_evenly_view
                   {
                       std::views::all(std::forward<viewable_range>(range)),
                       static_cast<range_diff<viewable_range>>(chunk_count)
                   }                                                          ))
        //
        ->         chunk_evenly_view<std::views::all_t<viewable_range>>
        //
        requires cxx::losslessly_convertible_to<integer_like,
                                                range_diff<viewable_range>>
        {
            return chunk_evenly_view
                   {
                       std::views::all(std::forward<viewable_range>(range)),
                       static_cast<range_diff<viewable_range>>(chunk_count)
                   };
        }
    };

    struct chunk_evenly_func
    {
        template <std::ranges::viewable_range viewable_range>
        //
        requires  cxx::ranges::sized_random_access_range<viewable_range>
        //
        [[nodiscard]]
        constexpr auto operator ()         (viewable_range&&  range,       const
            std::ranges::range_difference_t<viewable_range>   chunk_count) const
        //
        noexcept(
        noexcept(  chunk_evenly_view
                   {
                       std::views::all(std::forward<viewable_range>(range)),
                       chunk_count
                   }                                                          ))
        //
        ->         chunk_evenly_view<std::views::all_t<viewable_range>>
        {
            return chunk_evenly_view
                   {
                       std::views::all(std::forward<viewable_range>(range)),
                       chunk_count
                   };
        }

        [[nodiscard]]
        constexpr
        auto operator () (const cxx::maybe_integer_like auto chunk_count) const
        noexcept
        {
            return chunk_evenly_closure { chunk_count };
        }
    };

    inline constexpr auto chunk_evenly = chunk_evenly_func { };
}


#endif
