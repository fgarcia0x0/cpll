#include <iostream>
#include <cnf.hpp>
#include <token.hpp>
#include <unordered_map>
#include <cstring>

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

int main(int argc, char** argv)
{
    std::string input;
    input.append(1, '(');
    input.append(argv[1], std::strlen(argv[1]));
    input.append(1, ')');

    token_stream stream(input, "", "&>-#", connective_props_map);
    auto root = simplifier_cnf_expr(stream, connective_props_map);

    std::string s;
    to_expr_str(root, s, connective_props_map);
    std::cout << s << '\n';

    std::cout << "Finished\n";
}