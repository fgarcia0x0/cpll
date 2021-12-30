#pragma once

#include <token_stream.h>

namespace pll::grammar
{
    bool recognize(token_stream& stream);
}