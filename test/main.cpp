#include <iostream>
#include <string>
#include <token_stream.h>
#include <parser.h>
#include <vector>
#include <unordered_map>

using pll::token;
using pll::connective_properties;
using pll::operation_mode;
using pll::associativity;

static std::unordered_map<token, connective_properties> connective_props_map
{
    { token{'>', token::type::connective}, {0u, operation_mode::binary, associativity::left}},
    { token{'#', token::type::connective}, {1u, operation_mode::binary, associativity::left}},  
    { token{'&', token::type::connective}, {2u, operation_mode::binary, associativity::left}},
    { token{'-', token::type::connective}, {3u, operation_mode::unary,  associativity::left}}, 
    { token{'(', token::type::lparan},     {3u, operation_mode::none, associativity::none}}, 
    { token{')', token::type::rparan},     {3u, operation_mode::none, associativity::none}}, 
};

int main(int argc, const char* argv[])
{
    argc--, argv++;
   // ++argc;
   // argv[0] = "(a > b)";

    if (!argc || !argv)
        return EXIT_FAILURE;
    
    std::string input{ *argv };

    // concatenate all arguments
    for (int i = 1; i < argc; ++i)
        input += argv[i];

    // show input
    std::cout << input << '\n';

    pll::token_stream stream(input, "abcd", "&#->", connective_props_map);

    // pll::ast_parser parser(input, stream);
    // pll::ast_parser parser(input, stream);
    
    /*
    while (!stream.eos())
        std::cout << stream.next_token().value() << '\n';
    */
    
    std::cout << std::boolalpha << parser(stream) << '\n';
    return EXIT_SUCCESS;
}
