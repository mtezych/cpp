
#include <zlang/token.hxx>

#include <utility>

namespace zlang
{
    token::token (const keyword keyword) noexcept
    :
        state { keyword }
    {
    }

    token::token (const literal literal) noexcept
    :
        state { literal }
    {
    }

    token::token (const operator_ op) noexcept
    :
        state { op }
    {
    }

    token::token (identifier identifier)
    :
        state { std::move(identifier) }
    {
    }

    namespace
    {
        template <typename... Callables>
        struct overload : Callables...
        {
            using Callables::operator()...;
        };

        template <typename... Callables>
        overload(Callables...) -> overload<Callables...>;
    }

    auto str (const token& token) -> std::string
    {
        // @todo: Remove unnecessary std::string temporaries.

        const auto to_str = overload
        {
            [] (keyword keyword)
            {
                return "[ KEYWORD    ] " + std::string { str(keyword) };
            },
            [] (literal literal)
            {
                return "[ LITERAL    ] " + str(literal);
            },
            [] (operator_ op)
            {
                return "[ OPERATOR   ] " + std::string { str(op) };
            },
            [] (const identifier& identifier)
            {
                return "[ IDENTIFIER ] " + std::string { str(identifier) };
            },
        };

        return std::visit(to_str, token.state);
    }
}
