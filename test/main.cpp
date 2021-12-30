#include <iostream>
#include <string>
#include <unordered_map>
#include <token_stream.h>
#include <grammar.h>

using pll::token;
using pll::connective_properties;
using pll::operation_mode;
using pll::associativity;

static std::unordered_map<token, connective_properties> connective_props_map
{
    { token{'>', token::type::connective}, {0u, operation_mode::binary, associativity::right}},
    { token{'#', token::type::connective}, {1u, operation_mode::binary, associativity::left}},  
    { token{'&', token::type::connective}, {2u, operation_mode::binary, associativity::left}},
    { token{'-', token::type::connective}, {3u, operation_mode::unary,  associativity::left}}, 
    { token{'(', token::type::lparan},     {3u, operation_mode::none,   associativity::none}}, 
    { token{')', token::type::rparan},     {3u, operation_mode::none,   associativity::none}}, 
};

int main(int argc, const char* argv[])
{
    argc--, argv++;

    if (!argc || !argv)
    {
        std::cerr << "[-] invalid arguments, please run as: \"./plc expr\"\n";
        return EXIT_FAILURE;
    }
    
    std::string input{ *argv };

    for (int i = 1; i < argc; ++i)
        input += argv[i];

    std::cout << "[+] input: \"" << input << "\"\n";
    
    try
    {
        pll::token_stream tokenizer(input, "", "&#->", connective_props_map);
        bool valid = pll::grammar::recognize(tokenizer);

        if (!valid)
            std::cout << "[-] " << "invalid propositional formula\n";
        else
            std::cout << "[+] " << "valid propositional formula\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "[-] " << e.what() << '\n';
    }
    
    return EXIT_SUCCESS;
}
