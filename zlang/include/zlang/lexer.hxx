
#ifndef ZLANG_LEXER
#define ZLANG_LEXER

#include <zlang/token.hxx>

#include <optional>
#include <string_view>

namespace zlang
{
    class lexer
    {
        struct source_range
        {
            std::string_view::const_iterator begin;
            std::string_view::const_iterator end;
        };

        std::string_view::const_iterator current_char;
        std::string_view::const_iterator source_code_end;

        auto skip_whitespace_and_control_chars () -> void;
        auto ignore_comment                    () -> bool;
        auto lex_keyword_or_identifier         () -> std::optional<token>;
        auto lex_operator                      () -> std::optional<token>;
        auto lex_number                        () -> std::optional<token>;

    public:
        explicit lexer (std::string_view source_code);

        auto lex () -> std::optional<token>;
    };
}

#endif
