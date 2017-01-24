
#ifndef LIBRARY
#define LIBRARY

#include <string>
#include <cassert>

#if defined(__unix__)

	#include <dlfcn.h>

	namespace PlatformLibrary
	{
		using Handle = void*;

		Handle Init(const std::string& path);

		void Deinit(const Handle& handle);

		constexpr auto LoadSymbol = dlsym;
	}

#elif defined(_WIN32)

	#include <Windows.h>

	namespace PlatformLibrary
	{
		using Handle = HMODULE;

		Handle Init(const std::string& path);

		void Deinit(const Handle& handle);

		constexpr auto LoadSymbol = GetProcAddress;
	}

#else

	#error "Unsupported Platform"

#endif

class Library
{
private:

	PlatformLibrary::Handle handle;

public:

	Library();

	explicit Library(const std::string& path);

	~Library();

	Library(const Library& library) = delete;

	Library& operator =(const Library& library) = delete;

	Library(Library&& library);

	Library& operator =(Library&& library);

	template <typename SymbolType>
	SymbolType LoadSymbol(const std::string& symbolName) const
	{
		assert(handle != nullptr);

		const auto symbolAddress = PlatformLibrary::LoadSymbol
		(
			handle, symbolName.c_str()
		);
		assert(symbolAddress != nullptr);

		return reinterpret_cast<SymbolType>(symbolAddress);
	}
};

#endif
