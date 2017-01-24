
#include "library.h"

#if defined(__unix__)

	namespace PlatformLibrary
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

	namespace PlatformLibrary
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

Library::Library()
:
	handle { nullptr }
{
}

Library::Library(const std::string& path)
:
	handle { PlatformLibrary::Init(path) }
{
	assert(handle != nullptr);
}

Library::~Library()
{
	if (handle != nullptr)
	{
		PlatformLibrary::Deinit(handle);
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
		PlatformLibrary::Deinit(handle);
	}

	handle = library.handle;

	library.handle = nullptr;

	return *this;
}
