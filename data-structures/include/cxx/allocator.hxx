
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2024, Mateusz Zych
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


#ifndef CXX_ALLOCATOR
#define CXX_ALLOCATOR


#include <cxx/contracts.hxx>

#include <type_traits>

#include <cstddef>

#include <limits>

#include <new>


namespace cxx
{
    template <typename type> requires std::is_object_v<type>
    //
    class allocator
    {
        static_assert(sizeof(type) != 0, "incomplete types are not supported");

    public:
        using      value_type = type;
        using       size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        constexpr  allocator () noexcept = default;
        constexpr ~allocator () noexcept = default;

        constexpr allocator (const allocator&) noexcept               = default;
        constexpr
        auto     operator = (const allocator&) noexcept -> allocator& = default;

        template <typename other>
        constexpr explicit(false) allocator (const allocator<other>&) noexcept
        { }

        [[nodiscard]]
        auto allocate (const std::size_t count) -> type*
        {
            cxx_expects(count <= max_size());

            if constexpr (alignof(type) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            {
                constexpr auto alignment = std::align_val_t { alignof(type) };
                return
                static_cast<type*>(::operator new(count * sizeof(type), alignment));
            }
            else
            {
                return
                static_cast<type*>(::operator new(count * sizeof(type)));
            }
        }

        auto deallocate (type* const       pointer,
                         const std::size_t count) noexcept -> void
        {
            if (alignof(type) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            {
                constexpr auto alignment = std::align_val_t { alignof(type) };

                ::operator delete(pointer, count * sizeof(type), alignment);
            }
            else
            {
                ::operator delete(pointer, count * sizeof(type));
            }
        }

        [[nodiscard]]
        constexpr auto max_size () const noexcept -> std::size_t
        {
            constexpr auto ptrdiff_max = std::numeric_limits<std::ptrdiff_t>::max();
            constexpr auto    size_max = std::numeric_limits<std::   size_t>::max();

            constexpr auto is_ptrdiff_max_less_than_size_max =
                                        std::numeric_limits<std::ptrdiff_t>::digits
                                      < std::numeric_limits<std::   size_t>::digits;

            if constexpr (is_ptrdiff_max_less_than_size_max)
                         { return std::size_t { ptrdiff_max } / sizeof(type); }
            else         { return                  size_max   / sizeof(type); }
        }
    };

    template<typename left_type, typename right_type>
    //
    constexpr auto operator == (const allocator< left_type>&,
                                const allocator<right_type>&) noexcept -> bool
    {
        return true;
    }

    template<typename left_type, typename right_type>
    //
    constexpr auto operator != (const allocator< left_type>&,
                                const allocator<right_type>&) noexcept -> bool
    {
        return false;
    }
}


#endif
