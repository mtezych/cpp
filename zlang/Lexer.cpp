
#include "Lexer.h"

#include <cctype>
// ASCII           -> http://en.cppreference.com/w/cpp/language/ascii
// Character Types -> http://en.cppreference.com/w/cpp/string/byte

namespace
{

}

namespace zlang
{
	Lexer::Lexer (const std::string& code)
	:
		code    { std::move(code)     },
		codeIt  { this->code.cbegin() }
	{
	}

	// @todo: rewrite Lexer using regular expressions
	// @note: use std::regex or implement finite-state machine

	void Lexer::skipWhitespacesAndControlCodes()
	{
		while (codeIt != code.end())
		{
			if (!std::isspace(*codeIt) && !std::iscntrl(*codeIt))
			{
				break;
			}
			++codeIt;
		}
	}

	bool Lexer::ignoreComment()
	{
		// [#].*[\n\r]

		if ((codeIt != code.end()) && (*codeIt == '#'))
		{
			while (codeIt != code.end())
			{
				if ((*codeIt == '\n') || (*codeIt == '\r'))
				{
					break;
				}
				++codeIt;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	Token Lexer::parseKeywordOrIdentifier()
	{
		// [a-zA-Z][a-zA-Z0-9]*

		if ((codeIt != code.end()) && std::isalpha(*codeIt))
		{
			auto identifierName = std::string { *codeIt };
			++codeIt;

			while ((codeIt != code.end()) && std::isalnum(*codeIt))
			{
				identifierName += *codeIt;
				++codeIt;
			}

			if (identifierName == "def")
			{
				return Token { Token::KEYWORD, Keyword::DEF };
			}
			else if (identifierName == "extern")
			{
				return Token { Token::KEYWORD, Keyword::EXTERN };
			}
			else if (identifierName == "if")
			{
				return Token { Token::KEYWORD, Keyword::IF };
			}
			else if (identifierName == "else")
			{
				return Token { Token::KEYWORD, Keyword::ELSE };
			}
			else
			{
				return Token { Token::IDENTIFIER, identifierName };
			}
		}
		else
		{
			return Token { Token::INVALID };
		}
	}

	Token Lexer::parseOperator()
	{
		// [!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~]

		if ((codeIt != code.end()) && ispunct(*codeIt))
		{
			const auto operatorSymbol = *codeIt;
			++codeIt;

			return Token { Token::OPERATOR, operatorSymbol };
		}
		else
		{
			return Token { Token::INVALID };
		}
	}

	Token Lexer::parseNumber()
	{
		// ([0-9]+[.]?[0-9]*)|([.][0-9]+)

		if ((codeIt != code.end()) && std::isdigit(*codeIt))
		{
			auto numberDigits = std::string { *codeIt };
			++codeIt;

			while ((codeIt != code.end()) && std::isdigit(*codeIt))
			{
				numberDigits += *codeIt;
				++codeIt;
			}

			if ((codeIt != code.end()) && (*codeIt == '.'))
			{
				numberDigits += *codeIt;
				++codeIt;
			}

			while ((codeIt != code.end()) && std::isdigit(*codeIt))
			{
				numberDigits += *codeIt;
				++codeIt;
			}

			return Token { Token::LITERAL, std::stod(numberDigits) };
		}
		else if ((codeIt != code.end()) && (*codeIt == '.'))
		{
			const auto nextIt = std::next(codeIt);

			if ((nextIt != code.end()) && std::isdigit(*nextIt))
			{
				auto numberDigits = std::string { *codeIt };
				++codeIt;

				while ((codeIt != code.end()) && std::isdigit(*codeIt))
				{
					numberDigits += *codeIt;
					++codeIt;
				}

				return Token { Token::LITERAL, std::stod(numberDigits) };
			}
			else
			{
				return Token { Token::INVALID };
			}
		}
		else
		{
			return Token { Token::INVALID };
		}
	}

	Token Lexer::TokenizeNext()
	{
		skipWhitespacesAndControlCodes();

		if (ignoreComment())
		{
			return TokenizeNext();
		}

		auto token = Token { Token::INVALID };

		if (!token)
		{
			token = parseKeywordOrIdentifier();
		}

		if (!token)
		{
			token = parseNumber();
		}

		if (!token)
		{
			token = parseOperator();
		}

		return token;
	}
}
