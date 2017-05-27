
#include "Token.h"

#include <utility>
#include <cassert>

namespace zlang
{
	std::ostream& operator << (std::ostream& os, const Keyword& keyword)
	{
		switch (keyword)
		{
			case Keyword::EXTERN :
			{
				os << "extern";
				break;
			}
			case Keyword::DEF :
			{
				os << "def";
				break;
			}
			case Keyword::IF :
			{
				os << "if";
				break;
			}
			case Keyword::ELSE :
			{
				os << "else";
				break;
			}
			default :
			{
				assert(false);
				break;
			}
		}

		return os;
	}

	Token::Token (const Type type)
	:
		type { type }
	{
		assert(type == Type ::INVALID);
	}

	Token::Token (const Type type, Keyword keywordType)
	:
		type        { type        },
		keywordType { keywordType }
	{
		assert(type == Type::KEYWORD);
	}

	Token::Token (const Type type, const double literalValue)
	:
		type         { type         },
		literalValue { literalValue }
	{
		assert(type == Type::LITERAL);
	}

	Token::Token (const Type type, const std::string& identifierName)
	:
		type           { type           },
		identifierName { identifierName }
	{
		assert(type == Type::IDENTIFIER);
	}

	Token::Token (const Type type, const char operatorChar)
	:
		type         { type         },
		operatorChar { operatorChar }
	{
		assert(type == Type::OPERATOR);
	}

	Token::~Token ()
	{
		switch (type)
		{
			case Type::INVALID :
			case Type::KEYWORD :
			case Type::LITERAL :
			case Type::OPERATOR :
			{
				// nothing to destroy
				break;
			}
			case Type::IDENTIFIER :
			{
				identifierName.~basic_string();
				break;
			}
			default :
			{
				assert(false);
				break;
			}
		}
	}

	Token::Token (const Token& token)
	:
		type { token.type }
	{
		switch (type)
		{
			case Type::INVALID :
			{
				// nothing to copy
				break;
			}
			case Type::KEYWORD :
			{
				keywordType    = token.keywordType;
				break;
			}
			case Type::LITERAL :
			{
				literalValue   = token.literalValue;
				break;
			}
			case Type::IDENTIFIER :
			{
				identifierName = token.identifierName;
				break;
			}
			case Type::OPERATOR :
			{
				operatorChar   = token.operatorChar;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}
	}

	Token::Token (Token&& token)
	:
		type { token.type }
	{
		token.type = Type::INVALID;

		switch (type)
		{
			case Type::INVALID :
			{
				// nothing to move
				break;
			}
			case Type::KEYWORD :
			{
				keywordType    = std::move(token.keywordType);
				break;
			}
			case Type::LITERAL :
			{
				literalValue   = std::move(token.literalValue);
				break;
			}
			case Type::IDENTIFIER :
			{
				identifierName = std::move(token.identifierName);
				break;
			}
			case Type::OPERATOR :
			{
				operatorChar   = std::move(token.operatorChar);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}
	}

	Token& Token::operator = (const Token& token)
	{
		type = token.type;

		switch (type)
		{
			case Type::INVALID :
			{
				// nothing to copy
				break;
			}
			case Type::KEYWORD :
			{
				keywordType    = token.keywordType;
				break;
			}
			case Type::LITERAL :
			{
				literalValue   = token.literalValue;
				break;
			}
			case Type::IDENTIFIER :
			{
				identifierName = token.identifierName;
				break;
			}
			case Type::OPERATOR :
			{
				operatorChar   = token.operatorChar;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		return *this;
	}

	Token& Token::operator = (Token&& token)
	{
		type = token.type;
		token.type = Type::INVALID;

		switch (type)
		{
			case Type::INVALID :
			{
				// nothing to move
				break;
			}
			case Type::KEYWORD :
			{
				keywordType    = std::move(token.keywordType);
				break;
			}
			case Type::LITERAL :
			{
				literalValue   = std::move(token.literalValue);
				break;
			}
			case Type::IDENTIFIER :
			{
				identifierName = std::move(token.identifierName);
				break;
			}
			case Type::OPERATOR :
			{
				operatorChar   = std::move(token.operatorChar);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		return *this;
	}

	Token::operator bool() const
	{
		return type != Token::INVALID;
	}

	std::ostream& operator << (std::ostream& os, const Token& token)
	{
		switch (token.type)
		{
			case Token::INVALID :
			{
				os << "[ INVALID    ]";
				break;
			}
			case Token::KEYWORD :
			{
				os << "[ KEYWORD    ]" << ' ' << token.keywordType;
				break;
			}
			case Token::LITERAL :
			{
				os << "[ LITERAL    ]" << ' ' << token.literalValue;
				break;
			}
			case Token::IDENTIFIER :
			{
				os << "[ IDENTIFIER ]" << ' ' << token.identifierName;
				break;
			}
			case Token::OPERATOR :
			{
				os << "[ OPERATOR   ]" << ' ' << token.operatorChar;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		return os;
	}
}
