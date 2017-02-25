
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

#include "library.h"

#if defined(__unix__)

	namespace NativeLibrary
	{
		Handle Init(const std::string& path)
		{
			return dlopen(path.c_str(), RTLD_NOW);
		}

		void Deinit(const Handle& handle)
		{
			const auto result = dlclose(handle);
			assert(result == 0);
		}
	}

#elif defined(_WIN32)

	namespace NativeLibrary
	{
		namespace
		{
			std::wstring convert_UTF8_to_UTF16(const std::string& stringUTF8)
			{
				const auto wideCharactersNeeded = MultiByteToWideChar
				(
					CP_UTF8, 0,
					&stringUTF8[0], stringUTF8.length(),
					nullptr, 0
				);
				assert(wideCharactersNeeded > 0);

				auto stringUTF16 = std::wstring(wideCharactersNeeded, u'\0');

				const auto wideCharactersWritten = MultiByteToWideChar
				(
					CP_UTF8, 0,
					&stringUTF8[0], stringUTF8.length(),
					&stringUTF16[0], stringUTF16.length()
				);
				assert(wideCharactersWritten > 0);

				return stringUTF16;
			};
		}

		Handle Init(const std::string& path)
		{
			return LoadLibraryW(convert_UTF8_to_UTF16(path).c_str());
		}

		void Deinit(const Handle& handle)
		{
			const auto result = FreeLibrary(handle);
			assert(result != 0);
		}
	}

#else

	#error "Unsupported Platform"

#endif

namespace platform
{
	Library::Library()
	:
		handle { nullptr }
	{
	}

	Library::Library(const std::string& path)
	:
		handle { NativeLibrary::Init(path) }
	{
		assert(handle != nullptr);
	}

	Library::~Library()
	{
		if (handle != nullptr)
		{
			NativeLibrary::Deinit(handle);
		}
	}

	Library::Library(Library&& library)
	:
		handle { library.handle }
	{
		library.handle = nullptr;
	}

	Library& Library::operator =(Library&& library)
	{
		if (handle != nullptr)
		{
			NativeLibrary::Deinit(handle);
		}

		handle = library.handle;

		library.handle = nullptr;

		return *this;
	}
}
