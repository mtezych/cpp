
#ifndef ZLANG_LEXER
#define ZLANG_LEXER

#include "Token.h"

namespace zlang
{
	class Lexer
	{
		std::string                 code;
		std::string::const_iterator codeIt;

		void skipWhitespacesAndControlCodes();

		bool ignoreComment();

		Token parseKeywordOrIdentifier();
		Token parseOperator();
		Token parseNumber();

	public:
		Lexer (const std::string& code);

		Token TokenizeNext();
	};
}

#endif
