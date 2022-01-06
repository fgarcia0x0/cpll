#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <token_stream.h>
#include <grammar.h>
#include <cnf.hpp>

using pll::token;
using pll::connective_properties;
using pll::operation_mode;
using pll::associativity;
using pll::connective_type;
using pll::grammar::recognize;
using pll::token_stream;
using namespace pll::cnf;

static std::unordered_map<token, connective_properties> connective_props_map
{
    { token{'>', token::type::connective}, {0u, connective_type::implication, operation_mode::binary, associativity::right}},
    { token{'#', token::type::connective}, {1u, connective_type::disjuntive,  operation_mode::binary, associativity::left}},  
    { token{'&', token::type::connective}, {2u, connective_type::conjuntive,  operation_mode::binary, associativity::left}},
    { token{'-', token::type::connective}, {3u, connective_type::negation,    operation_mode::unary,  associativity::left}}, 
    { token{'(', token::type::lparan},     {3u, connective_type::none,        operation_mode::none,   associativity::none}}, 
    { token{')', token::type::rparan},     {3u, connective_type::none,        operation_mode::none,   associativity::none}}, 
};

static std::array g_basic_formulas
{
    "((a > b) # -c)", "-------(a) & --b", "a & b & c > d # a",
    "a # b"
};

TEST(FormulaTest, BasicUnitTests) 
{
    for (auto formula : g_basic_formulas)
    {
        token_stream tokenizer(formula, "", "&#->", connective_props_map);
        EXPECT_EQ(recognize(tokenizer), true);
    }
}