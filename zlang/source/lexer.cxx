
#include <zlang/lexer.hxx>

#include <cctype>
// ASCII           -> http://en.cppreference.com/w/cpp/language/ascii
// Character Types -> http://en.cppreference.com/w/cpp/string/byte

#include <iterator>
#include <string>

namespace zlang
{
    lexer::lexer (const std::string_view source_code)
    :
        current_char    { std::cbegin(source_code) },
        source_code_end { std::cend  (source_code) }
    {
    }

    auto lexer::skip_whitespace_and_control_chars () -> void
    {
        while (current_char != source_code_end)
        {
            if (!std::isspace(*current_char) && !std::iscntrl(*current_char))
            {
                break;
            }
            ++current_char;
        }
    }

    auto lexer::ignore_comment () -> bool
    {
        // [#].*[\n\r]

        if ((current_char != source_code_end) && (*current_char == '#'))
        {
            while (current_char != source_code_end)
            {
                if ((*current_char == '\n') || (*current_char == '\r'))
                {
                    break;
                }
                ++current_char;
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    auto lexer::lex_keyword_or_identifier() -> std::optional<token>
    {
        // [a-zA-Z][a-zA-Z0-9]*

        if ((current_char != source_code_end) && std::isalpha(*current_char))
        {
            auto name = std::string { *current_char };
            ++current_char;

            while ((current_char != source_code_end) && std::isalnum(*current_char))
            {
                name += *current_char;
                ++current_char;
            }

            if (name == "def")
            {
                return token { keyword::DEF };
            }
            else if (name == "extern")
            {
                return token { keyword::EXTERN };
            }
            else if (name == "if")
            {
                return token { keyword::IF };
            }
            else if (name == "else")
            {
                return token { keyword::ELSE };
            }
            else
            {
                return token { identifier { name } };
            }
        }
        else
        {
            return std::nullopt;
        }
    }

    auto lexer::lex_operator () -> std::optional<token>
    {
        // [!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~]

        if ((current_char != source_code_end) && std::ispunct(*current_char))
        {
            const auto symbol = *current_char;
            ++current_char;

            return token { operator_ { symbol } };
        }
        else
        {
            return std::nullopt;
        }
    }

    auto lexer::lex_number () -> std::optional<token>
    {
        // ([0-9]+[.]?[0-9]*)|([.][0-9]+)

        if ((current_char != source_code_end) && std::isdigit(*current_char))
        {
            auto digits = std::string { *current_char };
            ++current_char;

            while ((current_char != source_code_end) && std::isdigit(*current_char))
            {
                digits += *current_char;
                ++current_char;
            }

            if ((current_char != source_code_end) && (*current_char == '.'))
            {
                digits += *current_char;
                ++current_char;
            }

            while ((current_char != source_code_end) && std::isdigit(*current_char))
            {
                digits += *current_char;
                ++current_char;
            }

            return token { literal { std::stof(digits) } };
        }
        else if ((current_char != source_code_end) && (*current_char == '.'))
        {
            const auto nextIt = std::next(current_char);

            if ((nextIt != source_code_end) && std::isdigit(*nextIt))
            {
                auto digits = std::string { *current_char };
                ++current_char;

                while ((current_char != source_code_end) && std::isdigit(*current_char))
                {
                    digits += *current_char;
                    ++current_char;
                }

                return token { literal { std::stof(digits) } };
            }
            else
            {
                return std::nullopt;
            }
        }
        else
        {
            return std::nullopt;
        }
    }

    auto lexer::lex () -> std::optional<token>
    {
        skip_whitespace_and_control_chars();

        if (ignore_comment())
        {
            return lex();
        }

        auto token = std::optional<zlang::token> { std::nullopt };

        if (!token)
        {
            token = lex_keyword_or_identifier();
        }

        if (!token)
        {
            token = lex_number();
        }

        if (!token)
        {
            token = lex_operator();
        }

        return token;
    }
}
