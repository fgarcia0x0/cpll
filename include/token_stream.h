#pragma once

#include <string_view>
#include <optional>
#include <vector>
#include <token.hpp>

using std::optional;
using std::string_view;
using std::vector;

namespace pll
{
    class token_stream
    {
    public:
        constexpr token_stream() = default;
        constexpr token_stream(const token_stream&) = default;
        constexpr token_stream(token_stream&&) = default;

        constexpr token_stream(string_view input, 
                               string_view alphabet, 
                               string_view connectives)
            : m_data_ptr{ input.begin() },
              m_alphabet{ alphabet },
              m_connectives{ connectives }
        {
        }
        
        // EndOfStream
        constexpr bool eos() noexcept
        {
            return *m_data_ptr == '\0';
        }

        optional<token> next_token() noexcept;

    private:
        mutable string_view::iterator m_data_ptr{};
        string_view m_alphabet{};
        string_view m_connectives{};
    };
}