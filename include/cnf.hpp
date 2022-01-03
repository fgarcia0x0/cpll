#pragma once

#include <vector>
#include <type_traits>

#include <token.hpp>
#include <bst_node.hpp>

namespace pll::cnf
{
    using detail::bst_node;

    struct rule_handle_case
    {
    };

    struct rule_double_negation : rule_handle_case
    {
    };

    struct rule_negation_distribution : rule_handle_case
    {
    };

    struct rule_disjunction_distribution : rule_handle_case
    {
    };

    struct rule_double_conjunction : rule_handle_case
    {
    };

    // default
    static void handle_case(bst_node** root_pptr, rule_handle_case) = delete;

    // Joao
    static void handle_case(bst_node** root_pptr, rule_double_negation)
    {

    }

    // Jonadabe
    static void handle_case(bst_node** root_pptr, rule_negation_distribution)
    {

    }

    // Jonadade
    static void handle_case(bst_node** root_pptr, rule_disjunction_distribution)
    {
    }

    // Zazac
    static void handle_case(bst_node** root_pptr, rule_double_conjunction)
    {
    }

    /*  
                    [Name Convention]
        or_rule         => parse_disjunction_rules
        neg_rules       => parse_negation_rules
        imp_rule        => parse_implication_rules
        morgan_rules    => apply_morgan_rules
        gen_tries       => prop_to_bst
        CNF_simplifier  => simplifier_cnf_expr
        show_expression => bst_to_expr
    */
}