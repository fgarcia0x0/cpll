#include <iostream>
#include <pll_lexer.h>

namespace pll
{
    void lexer_analisys(vector<const char *>&& input)
    {
        for (auto&& element : input)
            std::cout << element << '\n';
        std::cout.put('\n');
    }
}
