#pragma once

#include <map>
#include <vector>
#include <string_view>
#include <token.hpp>
#include <token_stream.h>

namespace pll
{
    std::map<char, char> parser(std::string_view input, 
                                token_stream& stream);
}