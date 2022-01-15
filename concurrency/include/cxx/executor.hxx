
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


#ifndef CXX_EXECUTOR
#define CXX_EXECUTOR


#include <concepts>


namespace cxx
{
    // [GitHub] - Barry Revzin: Things you can almost, mostly, do with Concepts
    // ~ https://medium.com/p/6b52ace78630?source=brevzin.github.io
    //
    // [Andrzej's C++ blog] - Andrzej Krzemienski: Concept archetypes
    // ~ https://akrzemi1.wordpress.com/2020/09/02/concept-archetypes
    //
    namespace archetype
    {
        // note: Support for callable types, invocable with a set of arguments,
        //       is not implemented, since it's incredibly difficult to define
        //       an archetype for generic std::invocable<args_types...> concept.
        //
        //       [C++ reference] - std::invocable
        //       ~ https://en.cppreference.com/w/cpp/concepts/invocable
        //
        struct invocable
        {
            invocable () = delete;
           ~invocable () = delete;

            invocable (const invocable& ) = delete;
            invocable (      invocable&&) = delete;

            auto operator = (const invocable& ) -> invocable& = delete;
            auto operator = (      invocable&&) -> invocable& = delete;

            auto operator & () const -> const invocable* = delete;

            friend auto operator , (const auto&  left,
                                    const auto& right) -> decltype(right)
            //
            requires (std::same_as<decltype( left), const invocable&> ||
                      std::same_as<decltype(right), const invocable&>) = delete;

            auto operator () () -> void;
        };

        static_assert(std::invocable<cxx::archetype::invocable>);
        //
        // [ISO C++] - P0443R14: A Unified Executors Proposal for C++
        // ~ http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p0443r14.html
        //
        // [GitHub] - Eric Niebler: Partial header <execution> for P0443R12
        // ~ https://gist.github.com/ericniebler/8cc25656a0a496bd682edc8314d9576b#file-execution-h-L75
        //
        // [Boost] - Christopher Kohlhoff: Asio
        // ~ https://www.boost.org/doc/libs/1_78_0/doc/html/boost_asio/reference/execution__invocable_archetype.html
        // ~ https://www.boost.org/doc/libs/1_78_0/boost/asio/execution/invocable_archetype.hpp
    }


    template <typename type>
    concept executor = requires (type executor, cxx::archetype::invocable task)
                       {
                           { executor.submit(task) } -> std::same_as<void>;
                       };
}


#endif
