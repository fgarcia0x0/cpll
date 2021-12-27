#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <ostream>

namespace pll
{   
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

        char value;
        type type;

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

        friend auto& operator <<(std::ostream& os, token input)
        {
            os << "{" << input.value << ", " << input.type_str() << "}";
            return os;
        }
    };
}