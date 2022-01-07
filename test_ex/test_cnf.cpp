#include <iostream>
#include <token.hpp>
#include <unordered_map>
#include <cstring>
#include <cnf.h>
#include <string>
#include <grammar.h>

using namespace pll::cnf;
using namespace pll;

static std::unordered_map<token, connective_properties> connective_props_map
{
    { token{'>', token::type::connective}, {0u, connective_type::implication, operation_mode::binary, associativity::right}},
    { token{'#', token::type::connective}, {1u, connective_type::disjuntive,  operation_mode::binary, associativity::left}},  
    { token{'&', token::type::connective}, {2u, connective_type::conjuntive,  operation_mode::binary, associativity::left}},
    { token{'-', token::type::connective}, {3u, connective_type::negation,    operation_mode::unary,  associativity::left}}, 
    { token{'(', token::type::lparan},     {3u, connective_type::none,        operation_mode::none,   associativity::none}}, 
    { token{')', token::type::rparan},     {3u, connective_type::none,        operation_mode::none,   associativity::none}}, 
};

static void remove_atom_parenthesis(std::string& input)
{
    auto iter = input.begin();

    for(; iter != input.end(); ++iter)
    {
        if (input.size() < 3)
            break;
            
        if (*iter == '(' && *(iter + 2) == ')')
        {
            iter = input.erase(iter);
            ++iter;

            if (iter != input.end())
                iter = input.erase(iter);
        }
    }

    iter = input.begin();
}

int main(int argc, char** argv)
{
    --argc, ++argv;
    
    if (!argc || !argv)
    {
        std::cerr << "[+] no input passed\n";
        exit(EXIT_FAILURE);
    }

    std::string input;

    input.append(1, '(');
    input.append(*argv, std::strlen(*argv));
    input.append(1, ')');

    remove_atom_parenthesis(input);
    std::cout << "[+] input: " << input << '\n';

    token_stream stream_grammar(input, "", "&>-#", connective_props_map);
    
    try
    {
        if (!pll::grammar::recognize(stream_grammar))
        {
            std::cout << "[-] invalid proposition\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }

    try
    {
        token_stream stream(input, "", "&>-#", connective_props_map);
        std::string result = prop_to_cnf(stream, connective_props_map);
        std::cout << "[+] output: " << result << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "[-] Error: " << e.what() << '\n';
    }

    exit(EXIT_SUCCESS);
}