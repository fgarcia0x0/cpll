#include <iostream>
#include <cnf.hpp>
#include <token.hpp>
#include <unordered_map>

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

int main()
{
    // (p > q) & (q > r)
    /*
    bst_node* root = new bst_node(token('&', token::token_type::connective));
    root->left = new bst_node(token('>', token::token_type::connective));
    root->left->left = new bst_node(token('p', token::token_type::atom));
    root->left->right = new bst_node(token('q', token::token_type::atom));

    root->right = new bst_node(token('>', token::token_type::connective));
    root->right->left = new bst_node(token('q', token::token_type::atom)); 
    root->right->right = new bst_node(token('r', token::token_type::atom));
    */
    
    // -(-(p & q) or --p)

    std::string input{ "(-(p > q))" };
    token_stream stream(input, "", "&>-#", connective_props_map);
    auto new_root = simplifier_cnf_expr(stream, connective_props_map);

    std::string s;
    to_expr_str(new_root, s, connective_props_map);
    std::cout << s << '\n';

    std::cout << "Finished\n";

    // tem q retornar isso aq:
    // ENTRADA: ((p & q) # (q & r)) # ((r & t) # (q))
    // SAIDA: (p # r) & (p#t) & (q#r) & (q#t)
}