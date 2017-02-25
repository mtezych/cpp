
#include "library.h"

#include <gtest/gtest.h>

TEST(LibraryTest, libc_sprintf)
{
#if defined(__ANDROID__)
	const auto path = "libc.so";
#elif defined(__gnu_linux__)
	const auto path = "libc.so.6";
#else
	#error "Unsupported Platform"
#endif

	const auto library = platform::Library { path };

	using sprintfType = int (*)(char* buffer, const char* format, ...);

	const auto sprintfFunction = library.LoadSymbol<sprintfType>("sprintf");

	char buffer[11] = { };
	const auto result = sprintfFunction(buffer, "string %1d %c", 6, '$');

	ASSERT_EQ(10, result);
	ASSERT_STREQ("string 6 $", buffer);
}
