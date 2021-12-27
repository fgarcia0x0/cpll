#include <iostream>
#include <string>
#include <token_stream.h>
#include <ast_parser.h>

int main(int argc, const char* argv[])
{
    argc--, argv++;

    if (!argc || !argv)
        return EXIT_FAILURE;
    
    std::string input{ *argv };

    for (int i = 1; i < argc; ++i)
        input += argv[i];

    std::cout << input << '\n';

    pll::token_stream stream(input, "abcd", "&#->");
    pll::ast_parser parser(input, stream);
    
    auto token_ptr = stream.next_token();

    for (; token_ptr.has_value(); token_ptr = stream.next_token())
    {
        std::cout << token_ptr.value() << '\n';
    }

    // bool is_wff = pll::verify_formula(parser);
    
    return EXIT_SUCCESS;
}
