#include <iostream>
#include <pll_lexer.h>

int main(int argc, const char* argv[])
{
    pll::lexer_analisys(vector<const char*>(argv, argv + argc));
    return EXIT_SUCCESS;
}
