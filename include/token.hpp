#pragma once

#include <string_view>
#include <cstdint>
#include <ostream>
#include <functional>
#include <unordered_map>

namespace pll
{   
    enum class operation_mode
    {
        unary,
        binary,
        none
    };

    enum class associativity
    {
        left,
        right,
        none
    };

    enum class connective_type
    {
        none,
        implication,
        conjuntive,
        disjuntive,
        negation
    };

    struct connective_properties
    {
        uint8_t precedence;
        connective_type type;
        operation_mode mode;
        associativity assoc;
    };

    struct token
    {
        enum class type
        {
            atom,
            connective,
            lparan,
            rparan,
            unknown
        };

        using token_type = type;

        char value;
        type type;

        constexpr token()
            : value{},
              type{}
        {
        };

        constexpr token(char v, token_type t) noexcept
            : value{v},
              type{t}
        {
        }

        constexpr std::string_view type_str() noexcept
        {
            const char* type_str = "";

            switch (type)
            {
                case type::atom:
                    type_str = "atom";
                    break;
                case type::connective:
                    type_str = "connective";
                    break;
                case type::lparan:
                    type_str = "lparan";
                    break;
                case type::rparan:
                    type_str = "rparan";
                    break;
                case type::unknown:
                    type_str = "unknown";
                    break;
            }

            return type_str;
        }

        friend auto& operator<<(std::ostream& os, token input)
        {
            os << "{" << input.value << ", " << input.type_str() << "}";
            return os;
        }

        friend constexpr bool operator==(const token& lhs, const token& rhs)
        {
            return (lhs.value == rhs.value) && (lhs.type == rhs.type);
        }

        friend constexpr bool operator!=(const token& lhs, const token& rhs)
        {
            return !(lhs == rhs);
        }
    };

    using connective_prop_map = std::unordered_map<token, connective_properties>;
}

// Generate struct hashing for unordered map
template <>
struct std::hash<pll::connective_properties>
{
    std::size_t operator()(const pll::connective_properties& conn_props) const noexcept
    {
        std::size_t h1 = std::hash<uint8_t>{}(conn_props.precedence);
        std::size_t h2 = std::hash<pll::operation_mode>{}(conn_props.mode);
        return h1 ^ (h2 << 1);
    }
};

template <>
struct std::hash<pll::token>
{
    std::size_t operator()(const pll::token& token) const noexcept
    {
        std::size_t h1 = std::hash<char>{}(token.value);
        std::size_t h2 = std::hash<pll::token::token_type>{}(token.type);
        return h1 ^ (h2 << 1);
    }
};
