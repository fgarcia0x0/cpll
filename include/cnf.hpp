#pragma once

#include <vector>
#include <type_traits>
#include <stack>

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
    static bst_node** handle_case(bst_node** root_pptr, rule_negation_distribution)
    {
        auto current_node = *root_pptr;
        char mode = current_node->left->value.value;
        if (current_node->value.value == '-' && current_node->left->value.value == '&' || current_node->left->value.value == '#')
        {
            current_node->left->value.value == '-';
            current_node->right = new bst_node(token('-',token::type::connective), nullptr, nullptr);
            if(mode == '#')
                current_node->value.value = '&';
            if(mode == '&')
                current_node->value.value = '#';
            current_node->right->left = current_node->left->right;
            current_node->left->right = nullptr;
            current_node->right->right = nullptr;
        }

        return &current_node; 

    }

    // Jonadade
    static void handle_case(bst_node** root_pptr, rule_disjunction_distribution)
    {
    }

    // Zazac
    static void handle_case(bst_node** root_pptr, rule_double_conjunction)
    {
    }

    static void parse_disjunction_rules(bst_node** root_pptr)
    {
        auto current_node = *root_pptr;

        std::stack<bst_node*> stack;
        stack.push(*root_pptr);

        while (!stack.empty())
        {
            current_node = stack.top();
            stack.pop();

            if (current_node->value.value == '#')
            {
                if (current_node->left->value.value  == '&' &&
                    current_node->right->value.value == '&')
                {
                    handle_case(&current_node, rule_double_conjunction());

                    stack.push(current_node->left);
                    stack.push(current_node->right);
                }
                else
                {
                    handle_case(&current_node, rule_disjunction_distribution());
                    
                    stack.push(current_node->left);
                    stack.push(current_node->right);
                }
            }
            else
                continue;
        }
    }

    static void simplifier_cnf_expr()
    {
        // With doubts about the arguments of the function
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