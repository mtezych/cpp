
#ifndef ZLANG_TOKEN
#define ZLANG_TOKEN

#include <zlang/keyword.hxx>
#include <zlang/literal.hxx>
#include <zlang/operator.hxx>
#include <zlang/identifier.hxx>

#include <variant>
#include <string>

namespace zlang
{
    struct token
    {
        std::variant<keyword, literal, operator_, identifier> state;

        explicit token (keyword       keyword) noexcept;
        explicit token (literal       literal) noexcept;
        explicit token (operator_          op) noexcept;
        explicit token (identifier identifier);
    };

    auto str (const token& token) -> std::string;
}

#endif
