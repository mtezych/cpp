
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


#ifndef CXX_OPTIONAL
#define CXX_OPTIONAL


#include <type_traits>

#include <utility>

#include <cassert>


namespace cxx
{
    struct nullopt_t
    {
    };

    inline constexpr auto nullopt = nullopt_t { };

    template <typename value_type>
    class optional
    {
    private:
        bool initialized;
        std::aligned_storage_t<sizeof(value_type), alignof(value_type)> storage;

    public:
        constexpr optional () noexcept
        :
            initialized { false }
        {
        }

        constexpr optional (nullopt_t) noexcept
        :
            initialized { false }
        {
        }

        ~optional () noexcept
        {
            reset();
        }

        optional (const optional& other)
        :
            initialized { other.initialized }
        {
            if (other)
            {
                new (&storage) value_type (other.value());
            }
        }

        auto operator = (const optional& other) -> optional&
        {
            reset();

            initialized = other.initialized;

            if (other)
            {
                new (&storage) value_type (other.value());
            }

            return *this;
        }

        optional (optional&& other)
        :
            initialized { other.initialized }
        {
            if (other)
            {
                new (&storage) value_type (std::move(other.value()));

                other.reset();
            }
        }

        auto operator = (optional&& other) -> optional&
        {
            reset();

            initialized = other.initialized;

            if (other)
            {
                new (&storage) value_type (std::move(other.value()));

                other.reset();
            }

            return *this;
        }

        auto operator = (nullopt_t) -> optional&
        {
            reset();

            return *this;
        }

        template <typename ... types>
        auto emplace (types&& ... args) -> value_type&
        {
            reset();

            new (&storage) value_type (std::forward<types>(args)...);

            initialized = true;

            return value();
        }

        auto reset () -> void
        {
            if (has_value())
            {
                value().~value_type();

                initialized = false;
            }
        }

        [[nodiscard]]
        constexpr auto has_value() const noexcept -> bool
        {
            return initialized;
        }

        explicit constexpr operator bool() const noexcept
        {
            return has_value();
        }

        constexpr auto value () noexcept -> value_type&
        {
            assert(has_value());

            return *static_cast<value_type*>(
                    static_cast<      void*>(&storage));
        }

        constexpr auto value () const noexcept -> const value_type&
        {
            assert(has_value());

            return *static_cast<const value_type*>(
                    static_cast<const       void*>(&storage));
        }

        constexpr auto operator * () noexcept -> value_type&
        {
            return value();
        }

        constexpr auto operator * () const noexcept -> const value_type&
        {
            return value();
        }

        constexpr auto operator -> () noexcept -> value_type*
        {
            return &value();
        }

        constexpr auto operator -> () const noexcept -> const value_type*
        {
            return &value();
        }

        void swap (optional& optional)
        {
            if (this->has_value() && optional.has_value())
            {
                std::swap(this->value(), optional.value());
            }
            else if (optional.has_value())
            {
                new (&this->storage) value_type (std::move(optional.value()));

                optional.value().~value_type();
            }
            else if (has_value())
            {
                new (&optional.storage) value_type (std::move(this->value()));

                this->value().~value_type();
            }

            std::swap(this->initialized, optional.initialized);
        }
    };

    template <typename value_type>
    constexpr
    auto operator == (const optional<value_type>& optional,
                                                 nullopt_t) noexcept -> bool
    {
        return !optional.has_value();
    }

    template <typename value_type>
    constexpr
    auto operator != (const optional<value_type>& optional,
                                                 nullopt_t) noexcept -> bool
    {
        return optional.has_value();
    }

    template <typename value_type>
    constexpr
    auto operator == (nullopt_t,
                      const optional<value_type>& optional) noexcept -> bool
    {
        return !optional.has_value();
    }

    template <typename value_type>
    constexpr
    auto operator != (std::nullptr_t,
                      const optional<value_type>& optional) noexcept -> bool
    {
        return optional.has_value();
    }

    template <typename value_type, typename... args_types>
    auto make_optional (args_types&&... args) -> optional<value_type>
    {
        auto opt = optional<value_type> { };

        opt.emplace(std::forward<args_types>(args)...);

        return opt;
    }
}


#endif
