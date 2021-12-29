#pragma once

#include <map>
#include <vector>
#include <string_view>
#include <token.hpp>
#include <token_stream.h>

namespace pll
{
    bool parser(token_stream& stream);
}