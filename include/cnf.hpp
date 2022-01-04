#pragma once

#include <vector>
#include <type_traits>
#include <stack>
#include <unordered_map>

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
    static void handle_case(bst_node** root_pptr, 
                            const connective_prop_map& conn_prop_map,
                            rule_handle_case) = delete;

    // Joao
    static void handle_case(bst_node** root_pptr, 
                            const connective_prop_map& conn_prop_map,
                            rule_double_negation)
    {

    }

    // Jonadabe
    static bst_node** handle_case(bst_node** root_pptr,
                                  const connective_prop_map& conn_prop_map,
                                  rule_negation_distribution)
    {
        auto current_node = *root_pptr;

        char mode = current_node->left->value.value;

        bool is_neg = is_connective_type(current_node->value, conn_prop_map, connective_type::negation);
        bool is_conj = is_connective_type(current_node->value, conn_prop_map, connective_type::conjuntive);
        bool is_disj = is_connective_type(current_node->value, conn_prop_map, connective_type::disjuntive);

        if (is_neg && (is_conj || is_disj))
        {
            current_node->left->value.value = '-';
            current_node->right = new bst_node(token('-',token::type::connective), nullptr, nullptr);
            
            if(mode == '#')
                current_node->value.value = '&';

            if(mode == '&')
                current_node->value.value = '#';

            current_node->right->left  = current_node->left->right;
            current_node->left->right  = nullptr;
            current_node->right->right = nullptr;
        }

        return &current_node; 

    }

    // Jonadade
    static bst_node** handle_case(bst_node** root_pptr,
                                  const connective_prop_map& conn_prop_map, 
                                  rule_disjunction_distribution)
    {
        auto current_node = *root_pptr;
        if(current_node->value.value = '#')
        {
            if(current_node->left->value.value == '&' && current_node->right->value.value != '&')
            {
                current_node->value.value = '&';
                current_node->left->value.value = '#';
                auto right_copy_tree = current_node->right->clone();
                auto new_tree = new bst_node(token('#',token::type::connective), nullptr, nullptr);

                current_node->right = new_tree;
                new_tree->right = current_node->right;
                current_node->right->left = current_node->left->right;
                current_node->left->right = right_copy_tree;
            }else if(current_node->right->value.value == '&' && current_node->left->value.value != '&')
            {
                current_node->value.value = '&';
                current_node->right->value.value = '#';
                auto right_copy_tree = current_node->left->clone();
                auto new_tree = new bst_node(token('#',token::type::connective), nullptr, nullptr);

                current_node->left = new_tree;
                new_tree->left = current_node->left;
                current_node->left->right = current_node->right->left;
                current_node->right->left = right_copy_tree;
            }
        }
        return &current_node;
    }

    // Zazac
    static void handle_case(bst_node** root_pptr,
                            const connective_prop_map& conn_prop_map,
                            rule_double_conjunction)
    {
        // (p and q) or (r and t) = (p or r) and (p or t) and (q or r) and (q or t)
        auto current_node = *root_pptr;

        if (current_node->value.value == '#')
        {

            if(current_node->left->value.value == '&' && current_node->right->value.value == '&')
            {
                current_node->value.value = '&';

                bst_node *list_of_nodes[4] = {NULL, NULL, NULL, NULL}; // setting node list
                for(int i = 0; i < 4; ++i)
                {
                   list_of_nodes[i] = new bst_node(token('#',token::type::connective), nullptr, nullptr);
                }

                list_of_nodes[0]->left = current_node->left->left->clone();
                list_of_nodes[0]->right = current_node->right->left->clone();
                list_of_nodes[1]->left = current_node->left->left->clone();
                list_of_nodes[1]->right = current_node->right->right->clone();
                list_of_nodes[2]->left = current_node->left->right->clone();
                list_of_nodes[2]->right = current_node->right->left->clone();
                list_of_nodes[3]->left = current_node->left->right->clone();
                list_of_nodes[3]->right = current_node->right->right->clone();

                current_node->left->left = list_of_nodes[0];
                current_node->left->right = list_of_nodes[1];
                current_node->right->left = list_of_nodes[2];
                current_node->right->right = list_of_nodes[3];
            }
        }
        *root_pptr = current_node;

    }

    static void parse_disjunction_rules(bst_node** root_pptr,
                                        const connective_prop_map& conn_prop_map)
    {
        auto current_node = *root_pptr;

        std::stack<bst_node *> stack;
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
                    handle_case(&current_node, conn_prop_map, rule_double_conjunction{});

                    stack.push(current_node->left);
                    stack.push(current_node->right);
                }
                else
                {
                    handle_case(&current_node, conn_prop_map, rule_disjunction_distribution{});
                    stack.push(current_node->left);
                    stack.push(current_node->right);
                }
            }
            else
                continue;
        }
    }

    static bst_node** prop_to_bst()
    {
        
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