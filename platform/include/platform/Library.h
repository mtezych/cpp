
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

#ifndef PLATFORM_LIBRARY
#define PLATFORM_LIBRARY

#include <string>
#include <cassert>

#if defined(__unix__) || defined(__APPLE__)

	#include <dlfcn.h>

#elif defined(_WIN32)

	#include <Windows.h>

#else

	#error "Unsupported Platform"

#endif

namespace platform
{
	struct NativeLibrary
	{

#if defined(__unix__) || defined(__APPLE__)

		using Handle = void*;
		using Symbol = void*;

#elif defined(_WIN32)

		using Handle = HMODULE;
		using Symbol = FARPROC;

#endif

		static Handle Init (const std::string& path);

		static void Deinit (const Handle& handle);

		static Symbol LoadSymbol (const Handle& handle, const char* symbolName);
	};

	class Library
	{
	private:

		NativeLibrary::Handle handle;

	public:

		explicit Library (const std::string& path);

		~Library ();

		Library (Library&& library);
		Library (const Library& library) = delete;

		Library& operator = (Library&& library);
		Library& operator = (const Library& library) = delete;

		template <typename SymbolType>
		SymbolType LoadSymbol (const std::string& symbolName) const
		{
			const auto symbolAddress = NativeLibrary::LoadSymbol
			(
				handle, symbolName.c_str()
			);
			assert(symbolAddress != nullptr);

			return reinterpret_cast<SymbolType>(symbolAddress);
		}
	};
}

#endif
