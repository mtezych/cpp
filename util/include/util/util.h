
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2018, mtezych
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

#ifndef UTIL_UTIL
#define UTIL_UTIL

#include <vector>
#include <string>
#include <cstddef>
#include <cassert>
#include <type_traits>

namespace util
{
	template <typename Type>
	const Type& ReinterpretBytes (const std::vector<std::byte>& bytes)
	{
		static_assert(std::is_scalar<Type>::value, "Type has to be scalar.");

		assert(bytes.size() == sizeof(Type));

		return *static_cast<const Type*>(static_cast<const void*>(bytes.data()));
	}

	std::string StringFromBytes (const std::vector<std::byte>& bytes);

	std::vector<std::string>
	Split (const std::string& string, const char delimiter);

	//
	// IsAnyBitSet(bitMask    : 0b00111010,
	//             bitPattern : 0b11010100) -> true
	//
	// IsAnyBitSet(bitMask    : 0b00111010,
	//             bitPattern : 0b11000100) -> false
	//
	constexpr
	bool IsAnyBitSet (const uint64_t bitMask, const uint64_t bitPattern)
	{
		return (bitMask & bitPattern) != 0;
	}

	template <typename EnumType>
	constexpr
	auto enum_cast (const EnumType enumValue)
	{
		static_assert(std::is_enum_v<EnumType>, "EnumType has to be enum.");

		using UnderlyingType = std::underlying_type_t<EnumType>;

		return static_cast<UnderlyingType>(enumValue);
	}
}

#endif
