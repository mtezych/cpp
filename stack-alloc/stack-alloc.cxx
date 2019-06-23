
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, mtezych
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


#if defined(__linux__)

    // [Linux] - alloca (size_t size) -> void*
    // ~ http://man7.org/linux/man-pages/man3/alloca.3.html
    #include <alloca.h>

    // [GCC] - Common Function Attributes
    // ~ https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
    #define always_inline __attribute__((always_inline)) inline

#elif defined(_WIN32)

    // [Microsoft CRT] - _alloca (size_t size) -> void*
    // ~ https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/alloca
    #include <malloc.h>

    // [Micrsoft Visual C++] - Inline Functions
    // ~ https://docs.microsoft.com/en-us/cpp/cpp/inline-functions-cpp
    #define always_inline __forceinline

#else

    #error "Unsupported platform!"

#endif


#include <cassert>

#include <cstddef>

namespace cxx
{
    //
    // The stack_alloc() function only allocates memory.
    // It does *not* create C++ objects under rules of the C++ abstract machine.
    //
    // Therefore, client code is required to
    // call placement new operator to create a C++ object.
    //
    // ~ https://groups.google.com/a/isocpp.org/d/msg/std-discussion/rt2ivJnc4hg/4RKYJeF5CQAJ
    //
    template <typename type>
    always_inline auto stack_alloc (const std::ptrdiff_t size) noexcept -> type*
    {
        assert(size > 0);

    #if defined(__linux__)

        return static_cast<type*>(alloca(sizeof(type) * size));

    #elif defined(_WIN32)

        return static_cast<type*>(_alloca(sizeof(type) * size));

    #endif
    }
}


#include <new>

namespace cxx
{
    template <typename element>
    class stack_array
    {
    private:

        element*       data_;
        std::ptrdiff_t size_;
        std::ptrdiff_t capacity_;

    public:

        always_inline stack_array (const std::ptrdiff_t capacity)
        :
            data_     { stack_alloc<element>(capacity) },
            size_     { 0                              },
            capacity_ { capacity                       }
        {
            assert(capacity > 0);
        }

        auto data () noexcept -> element*
        {
            return data_;
        }

        auto data () const noexcept -> const element*
        {
            return data_;
        }

        auto size () const noexcept -> std::ptrdiff_t
        {
            return size_;
        }

        auto capacity () const noexcept -> std::ptrdiff_t
        {
            return capacity_;
        }

        auto push (const element& value) -> void
        {
            new (data_ + size_) element(value);

            ++size_;
        }

        auto push (element&& value) -> void
        {
            new (data_ + size_) element(std::move(value));

            ++size_;
        }

        auto operator [] (const std::ptrdiff_t index) noexcept -> element&
        {
            assert((index >= 0) && (index < size_));

            return data_[index];
        }

        auto operator [] (const std::ptrdiff_t index) const noexcept -> const element&
        {
            assert((index >= 0) && (index < size_));

            return data_[index];
        }
    };
}


#include <algorithm>

#include <numeric>

namespace test
{
    auto stack_alloc () -> bool
    {
        struct span
        {
            int*           data;
            std::ptrdiff_t size;
        };

        const auto array = span { cxx::stack_alloc<int>(4096), 4096 };

        for (auto i = 0; i < array.size; ++i)
        {
            new (array.data + i) int { 1 };
        }

        // std::fill(array.data, array.data + array.size, 1);

        auto sum = std::accumulate(array.data, array.data + array.size, 0);

        return sum == array.size;
    }

    auto stack_array () -> bool
    {
        auto array = cxx::stack_array<int> { 4096 };

        for (auto i = 0; i < array.capacity(); ++i)
        {
            array.push(1);
        }

        // std::fill(array.data(), array.data() + array.size(), 1);

        auto sum = std::accumulate(array.data(), array.data() + array.size(), 0);

        return sum == array.size();
    }
}


#include <cstdlib>

auto main () -> int
{
    const auto validate = [] (const bool condition)
    {
        if (!condition)
        {
            std::exit(EXIT_FAILURE);
        }
    };

    validate(test::stack_alloc());
    validate(test::stack_array());

    return EXIT_SUCCESS;
}
