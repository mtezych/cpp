
#ifndef ZLANG_TOKEN
#define ZLANG_TOKEN

#include <string>
#include <ostream>

namespace zlang
{
	// @todo: replace Token implementation with std::variant

	enum class Keyword
	{
		EXTERN,
		DEF,
		IF,
		ELSE,
	};

	std::ostream& operator << (std::ostream& os, const Keyword& keyword);

	struct Token
	{
		enum Type
		{
			INVALID   ,
			KEYWORD   ,
			LITERAL   ,
			IDENTIFIER,
			OPERATOR  ,
		};

		Type type;

		union
		{
			Keyword     keywordType;
			double      literalValue;
			std::string identifierName;
			char        operatorChar;
		};

		Token (const Type type);
		Token (const Type type, const Keyword keyword);
		Token (const Type type, const double numberValue);
		Token (const Type type, const std::string& identifierName);
		Token (const Type type, const char operatorChar);

		~Token ();

		Token (const Token& token);
		Token (Token&& token);

		Token& operator = (const Token& token);
		Token& operator = (Token&& token);

		explicit operator bool() const;
	};

	std::ostream& operator << (std::ostream& os, const Token& token);
}

#endif
