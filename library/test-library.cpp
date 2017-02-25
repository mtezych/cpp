
#include "library.h"

#include <gtest/gtest.h>

TEST(LibraryTest, libc_sprintf)
{
	const auto library = Library { "libc.so.6" };

	using sprintfType = int (*)(char* buffer, const char* format, ...);

	const auto sprintfFunction = library.LoadSymbol<sprintfType>("sprintf");

	char buffer[11] = { };
	const auto result = sprintfFunction(buffer, "string %1d %c", 6, '$');

	ASSERT_EQ(10, result);
	ASSERT_STREQ("string 6 $", buffer);
}
