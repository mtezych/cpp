
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


#ifndef CXX_UNIQUE_PTR
#define CXX_UNIQUE_PTR


#include <utility>

#include <cassert>


namespace cxx
{
    template <typename value_type>
    class unique_ptr
    {
    private:
        value_type* ptr;

    public:
        constexpr unique_ptr () noexcept
        :
            ptr { nullptr }
        {
        }

        constexpr unique_ptr (std::nullptr_t) noexcept
        :
            ptr { nullptr }
        {
        }

        explicit constexpr unique_ptr (value_type* const raw_ptr) noexcept
        :
            ptr { raw_ptr }
        {
        }

        ~unique_ptr () noexcept
        {
            delete ptr;
        }

        unique_ptr (const unique_ptr&) = delete;

        auto operator = (const unique_ptr&) -> unique_ptr& = delete;

        constexpr unique_ptr (unique_ptr&& uptr) noexcept
        :
            ptr { uptr.ptr }
        {
            uptr.ptr = nullptr;
        }

        auto operator = (unique_ptr&& uptr) noexcept -> unique_ptr&
        {
            delete ptr;

            ptr = uptr.ptr;

            uptr.ptr = nullptr;

            return *this;
        }

        auto operator = (std::nullptr_t) noexcept -> unique_ptr&
        {
            delete ptr;

            ptr = nullptr;

            return *this;
        }

        constexpr auto operator * () const noexcept -> value_type&
        {
            assert(ptr != nullptr);

            return *ptr;
        }

        constexpr auto operator -> () const noexcept -> value_type*
        {
            return ptr;
        }

        [[nodiscard]]
        constexpr auto get () const noexcept -> value_type*
        {
            return ptr;
        }

        explicit constexpr operator bool () const noexcept
        {
            return ptr != nullptr;
        }

        [[nodiscard]]
        constexpr auto release () noexcept -> value_type*
        {
            const auto raw_ptr = ptr;

            ptr = nullptr;

            return raw_ptr;
        }

        auto reset (value_type* const raw_ptr = nullptr) -> void
        {
            delete ptr;

            ptr = raw_ptr;
        }
    };

    template <typename value_type>
    constexpr
    auto operator == (const unique_ptr<value_type>& uptr,
                                          std::nullptr_t) noexcept -> bool
    {
        return uptr.get() == nullptr;
    }

    template <typename value_type>
    constexpr
    auto operator != (const unique_ptr<value_type>& uptr,
                                          std::nullptr_t) noexcept -> bool
    {
        return uptr.get() != nullptr;
    }

    template <typename value_type>
    constexpr
    auto operator == (std::nullptr_t,
                      const unique_ptr<value_type>& uptr) noexcept -> bool
    {
        return nullptr == uptr.get();
    }

    template <typename value_type>
    constexpr
    auto operator != (std::nullptr_t,
                      const unique_ptr<value_type>& uptr) noexcept -> bool
    {
        return nullptr != uptr.get();
    }

    template <typename value_type>
    constexpr
    auto operator == (const unique_ptr<value_type>&  left_uptr,
                      const unique_ptr<value_type>& right_uptr) noexcept -> bool
    {
        return left_uptr.get() == right_uptr.get();
    }

    template <typename value_type>
    constexpr
    auto operator != (const unique_ptr<value_type>&  left_uptr,
                      const unique_ptr<value_type>& right_uptr) noexcept -> bool
    {
        return left_uptr.get() != right_uptr.get();
    }

    template <typename value_type, typename... args_types>
    auto make_unique (args_types&&... args) -> unique_ptr<value_type>
    {
        return unique_ptr<value_type>
               {
                   new value_type(std::forward<args_types>(args)...)
               };
    }
}


#endif
