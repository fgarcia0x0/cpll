#include <cstddef>
#include <cstring>
#include <algorithm>
#include <string>
#include <token_stream.h>

namespace pll
{
    std::optional<token> token_stream::current_token() noexcept
    {
        if (!this->eos())
        {
            uint8_t target = static_cast<uint8_t>(*m_data_ptr);
            auto token_type{ token::type::unknown };

            while (std::isspace(target) && !this->eos())
                target = static_cast<uint8_t>(*++m_data_ptr);

            if (this->eos())
                return std::nullopt;

            if ((m_alphabet.empty() && std::isalpha(target)) || 
                (!m_alphabet.empty() && std::strchr(m_alphabet.data(), target)))
            {
                token_type = token::type::atom;
            }
            else if (target == static_cast<uint8_t>('('))
            {
                token_type = token::type::lparan;
            }
            else if (target == static_cast<uint8_t>(')'))
            {
                token_type = token::type::rparan;
            }
            else if (std::strchr(m_connectives.data(), target))
            {
                token_type = token::type::connective;
            }

            return token{ *m_data_ptr, token_type };
        }

        return std::nullopt;
    }

    std::optional<token> token_stream::next_token() noexcept
    {
        std::optional<token> current = current_token();
        ++m_data_ptr;
        return current;
    }
}
