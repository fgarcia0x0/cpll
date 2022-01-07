#pragma once

#include <vector>
#include <type_traits>
#include <stack>
#include <unordered_map>
#include <array>
#include <optional>
#include <iterator>
#include <utils.hpp>
#include <cassert>
#include <string>

#include <token.hpp>
#include <token_stream.h>
#include <bst_node.hpp>

namespace pll::cnf
{
    using detail::bst_node;
    
    struct bst_node_wrapper
    {
        bst_node* node;
        bool is_ptr_ref;

        static bst_node_wrapper make(bst_node* curr, bool ptr_ref)
        {
            return bst_node_wrapper{ curr, ptr_ref };
        }
    };
    
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
        bst_node* curr = *root_pptr;

        bool is_neg        = is_connective_type(curr->value, conn_prop_map, connective_type::negation);
        bool is_double_neg = is_connective_type(curr->left->value, conn_prop_map, connective_type::negation);

        if(is_neg && is_double_neg)
        {
            curr->value = curr->left->left->value;
            curr->right = curr->left->left->right;
            curr->left  = curr->left->left->left;
        }

        *root_pptr = curr;
    }

    // Jonadabe
    static void handle_case(bst_node** root_pptr,
                            const connective_prop_map& conn_prop_map,
                            rule_negation_distribution)
    {
        bst_node* curr = *root_pptr;
        auto mode = conn_prop_map.find(curr->left->value)->second.type;
        auto conn_neg = extract_token(connective_type::negation, conn_prop_map);

        bool is_neg  = is_connective_type(curr->value, conn_prop_map, connective_type::negation);
        bool is_conj = is_connective_type(curr->left->value, conn_prop_map, connective_type::conjuntive);
        bool is_disj = is_connective_type(curr->left->value, conn_prop_map, connective_type::disjuntive);

        if (is_neg && (is_conj || is_disj))
        {
            curr->left->value = conn_neg;
            curr->right = new bst_node(conn_neg, nullptr, nullptr);
            
            if (mode == connective_type::disjuntive)
                curr->value = extract_token(connective_type::conjuntive, conn_prop_map);

            if (mode == connective_type::conjuntive)
                curr->value = extract_token(connective_type::disjuntive, conn_prop_map);

            curr->right->left  = curr->left->right;
            curr->left->right  = nullptr;
            curr->right->right = nullptr;
        }

        *root_pptr = curr;
    }

    static void simplify_neg_rules(bst_node** root_pptr, 
                                   const connective_prop_map& conn_prop_map)
    {
        auto curr_tree = *root_pptr;

        std::stack<bst_node *> stack;
        stack.push(curr_tree);

        auto is_conn_type = [&conn_prop_map](token target, connective_type type)
        {
            return is_connective_type(target, conn_prop_map, type);
        };

        while (!stack.empty())
        {
            curr_tree = stack.top();
            stack.pop();

            if (is_conn_type(curr_tree->value, connective_type::negation))
            {
                if (is_conn_type(curr_tree->left->value, connective_type::negation))
                {
                    handle_case(&curr_tree, conn_prop_map, rule_double_negation{});
                    stack.push(curr_tree);
                }
                else if (is_conn_type(curr_tree->left->value, connective_type::conjuntive) ||
                         is_conn_type(curr_tree->left->value, connective_type::disjuntive))
                {
                    handle_case(&curr_tree, conn_prop_map, rule_negation_distribution{});
                    stack.push(curr_tree->left);
                    stack.push(curr_tree->right);
                }
            }
        }
    }

    // Jonadade
    static void handle_case(bst_node** root_pptr,
                            const connective_prop_map& conn_prop_map, 
                            rule_disjunction_distribution)
    {
        auto curr = *root_pptr;
        
        bool is_conn = is_connective_type(curr->value,
                                          conn_prop_map,
                                          connective_type::disjuntive);
        
        if (is_conn)
        {
            auto is_conn_left  = is_connective_type(curr->left->value, 
                                                    conn_prop_map, 
                                                    connective_type::conjuntive);
                                                    
            auto is_conn_right = is_connective_type(curr->right->value, 
                                                    conn_prop_map, 
                                                    connective_type::conjuntive);
            
            if (is_conn_left && !is_conn_right)
            {
                curr->value = extract_token(connective_type::conjuntive, 
                                            conn_prop_map);

                curr->left->value = extract_token(connective_type::disjuntive, 
                                                  conn_prop_map);
                
                auto right_copy_tree = curr->right->clone();
                auto new_tree = new bst_node(curr->left->value, nullptr, nullptr);

                auto temp = curr->right;
                curr->right = new_tree;
                new_tree->right = temp;

                curr->right->left = curr->left->right;
                curr->left->right = right_copy_tree;
            }
            else if (is_conn_right && !is_conn_left)
            {
                curr->value = extract_token(connective_type::conjuntive, 
                                            conn_prop_map);
                                                    
                curr->right->value = extract_token(connective_type::disjuntive, 
                                                   conn_prop_map);

                auto right_copy_tree = curr->left->clone();
                auto new_tree = new bst_node(curr->right->value, nullptr, nullptr);

                auto temp = curr->left;
                curr->left = new_tree;
                new_tree->left = temp;

                curr->left->right = curr->right->left;
                curr->right->left = right_copy_tree;
            }
        }

        *root_pptr = curr;
    }

    // Zazac
    static void handle_case(bst_node** root_pptr,
                            const connective_prop_map& conn_prop_map,
                            rule_double_conjunction)
    {
        // (p&q) # (r&t) ==> (p#r) & (p#t) & (q#r) & (q#t)
        auto curr = *root_pptr;
        auto conn_type = is_connective_type(curr->value, conn_prop_map, connective_type::disjuntive);
        
        if (conn_type)
        {
            auto is_conn_left  = is_connective_type(curr->left->value, 
                                                    conn_prop_map, 
                                                    connective_type::conjuntive);
                                                    
            auto is_conn_right = is_connective_type(curr->right->value, 
                                                    conn_prop_map, 
                                                    connective_type::conjuntive);
                                                    
            if(is_conn_left && is_conn_right)
            {
                curr->value = extract_token(connective_type::conjuntive,
                                            conn_prop_map);
                
                std::array<bst_node *, 4> tree_list;

                auto conn_or = extract_token(connective_type::disjuntive, conn_prop_map);
                for (auto& tree : tree_list)
                    tree = new bst_node(conn_or, nullptr, nullptr);

                tree_list[0]->left  = curr->left->left->clone();
                tree_list[0]->right = curr->right->left->clone();

                tree_list[1]->left  = curr->left->left->clone();
                tree_list[1]->right = curr->right->right->clone();

                tree_list[2]->left  = curr->left->right->clone();
                tree_list[2]->right = curr->right->left->clone();
                
                tree_list[3]->left  = curr->left->right->clone();
                tree_list[3]->right = curr->right->right->clone();

                curr->left->left   = tree_list[0];
                curr->left->right  = tree_list[1];
                
                curr->right->left  = tree_list[2];
                curr->right->right = tree_list[3];
            }
        }
        
        *root_pptr = curr;
    }

    // TODO: Testar
    static void simplify_disj_rules(bst_node** root_pptr,
                                    const connective_prop_map& conn_prop_map)
    {
        auto curr = *root_pptr;

        std::stack<bst_node *> stack;
        stack.push(curr);

        while (!stack.empty())
        {
            curr = stack.top();
            stack.pop();

            bool is_conn = is_connective_type(curr->value,
                                              conn_prop_map,
                                              connective_type::disjuntive);

            if (is_conn)
            {
                auto is_conn_left  = is_connective_type(curr->left->value, 
                                                        conn_prop_map, 
                                                        connective_type::conjuntive);
                                                    
                auto is_conn_right = is_connective_type(curr->right->value,
                                                        conn_prop_map, 
                                                        connective_type::conjuntive);

                if (is_conn_left && is_conn_right)
                    handle_case(&curr, conn_prop_map, rule_double_conjunction{});
                else
                    handle_case(&curr, conn_prop_map, rule_disjunction_distribution{});

                stack.push(curr->left);
                stack.push(curr->right);
            }
        }
    }

    static void simplify_imp_rules(bst_node** root_pptr, const connective_prop_map& conn_prop_map)
    {
        auto curr = *root_pptr;
        auto conn_neg = extract_token(connective_type::negation, conn_prop_map);
        auto is_impl = is_connective_type(curr->value, conn_prop_map, connective_type::implication);

        if (is_impl)
        {
            curr->value = extract_token(connective_type::disjuntive, conn_prop_map);
            
            bst_node* new_tree = new bst_node(conn_neg);
            std::swap(curr->left, curr->right);

            new_tree->left = curr->right;
            curr->right = new_tree;

            simplify_neg_rules(&(curr->right), conn_prop_map);
            simplify_disj_rules(&curr, conn_prop_map);
        }

        *root_pptr = curr;
    }

    static void apply_cnf_morgan_rules(bst_node** root_pptr, const connective_prop_map& conn_prop_map)
    {
        auto curr = *root_pptr;

        auto is_conjunction = is_connective_type(curr->value, conn_prop_map, connective_type::conjuntive);
        auto connective = curr->value;

        if (!is_conjunction)
        {   
            if (is_connective_type(connective, conn_prop_map, connective_type::implication))
                simplify_imp_rules(&curr, conn_prop_map);
            else if (is_connective_type(connective, conn_prop_map, connective_type::negation))
                simplify_neg_rules(&curr, conn_prop_map);
            else if (is_connective_type(connective, conn_prop_map, connective_type::disjuntive))
                simplify_disj_rules(&curr, conn_prop_map);
        }
    }

    static bst_node* prop_to_bst(bst_node* left_expr, 
                                 bst_node* root,
                                 bst_node* right_expr)    
    {
        bst_node* new_tree = new bst_node(*root);
        new_tree->left  = left_expr;
        new_tree->right = right_expr;
        return new_tree;
    }

    static bst_node* simplifier_cnf_expr(token_stream& stream, 
                                         const connective_prop_map& conn_prop_map)
    {
        std::vector<std::optional<token>> prop_list;
        stream.write(std::back_inserter(prop_list));

        std::stack<bst_node*> stack;
        for (const auto& token_opt : prop_list)
        {
            assert(token_opt);
            token target = token_opt.value();
            
            if (target.type != token::token_type::rparan)
                stack.push(new bst_node(target));
            else
            {
                auto right_expr = stack.top();
                if (!stack.empty())
                    stack.pop();
                
                if (stack.empty())
                    break;

                auto connective = stack.top();
                if (!stack.empty())
                    stack.pop();
                
                if (stack.empty())
                    break;

                auto left_expr = stack.top();
                if (!stack.empty())
                    stack.pop();

                bst_node* symbol{}; 

                if (connective->value.type != token::token_type::connective)
                {
                    throw std::invalid_argument{"target token must be a connective type"};
                }
                else if (!is_connective_type(connective->value, conn_prop_map, connective_type::negation))
                {
                    symbol = stack.top();
                    if (!stack.empty())
                        stack.pop();
                }
                else
                {
                    symbol = left_expr;
                }

                if (symbol->value.type != token::token_type::lparan)
                {
                    throw std::invalid_argument("The expression waited for a parenthesis");
                }
                else
                {
                    bst_node* new_tree = nullptr;

                    if (symbol == left_expr)
                        new_tree = prop_to_bst(right_expr, connective, nullptr);
                    else
                        new_tree = prop_to_bst(left_expr, connective, right_expr);
                    
                    apply_cnf_morgan_rules(&new_tree, conn_prop_map);
                    stack.push(new_tree);
                }
            }
        }

        if (stack.empty())
            return nullptr;
        
        auto cnf_tree_root = stack.top();
        if (!stack.empty())
            stack.pop();
        
        if (!stack.empty())
            return nullptr;
        
        return cnf_tree_root;
    }

    // TEM Q COMENTAR ESSA PORRA
    static void to_expr_str(bst_node* root, 
                            std::string& str, 
                            const connective_prop_map& conn_prop_map)
    {
        if (!root)
            return;

        if (!root->left && !root->right)
        {
            str += root->value.value;
            return;
        }
        
        auto conn_neg = extract_token(connective_type::negation, conn_prop_map);
        if (root->value == conn_neg)
            str += root->value.value;

        if (root->left)
        {
            if (root->left->left || root->left->right)
                str += '(';
            
            to_expr_str(root->left, str, conn_prop_map);

            if (root->left->left || root->left->right)
                str += ')';
        }

        if (root->value.type == token::token_type::connective && 
            root->value != conn_neg)
        {
            str += ' ';
            str += root->value.value;
            str += ' ';
        }

        if (root->right)
        {
            if (root->right->left || root->right->right)
                str += '(';

            to_expr_str(root->right, str, conn_prop_map);

            if (root->right->left || root->right->right)
                str += ')';
        }
    }
}