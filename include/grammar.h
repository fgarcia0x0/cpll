#pragma once

#include <token_stream.h>

namespace pll::grammar
{
    /**
     * @brief Verifies if a propositional formula is valid or not
     * 
     * @param stream a stream of tokens of prop. formula
     * @return true if valid formula
     */
    bool recognize(token_stream& stream);
}