#pragma once

#include <token_stream.h>
#include <cnf.hpp>

namespace pll::grammar
{
    bool recognize(token_stream& stream);
}