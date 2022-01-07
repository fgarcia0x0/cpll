#pragma once

#include <queue>
#include <token.hpp>

namespace pll::detail
{
    struct bst_node
    {
        token value;
        bst_node* left;
        bst_node* right;

        constexpr bst_node()
            : value{},
              left{},
              right{}
        {
        };

        bst_node(const bst_node&) = default;
        bst_node& operator = (const bst_node&) = default;

        constexpr bst_node(const token token, bst_node* lhs = nullptr, bst_node* rhs = nullptr)
            : value{token},
              left{lhs},
              right{rhs}
        {
        }

        bst_node* clone() const
        {
            return new bst_node(*this);
        }
    };

    static void bst_release(bst_node* root)
    {
        std::queue<bst_node *> qnodes;
        qnodes.push(root);

        while (!qnodes.empty())
        {
            bst_node* temp = qnodes.front();
            qnodes.pop();
                
            if (temp->left)
                qnodes.push(temp->left);
                    
            if (temp->right)
                qnodes.push(temp->right);

            delete temp;
        }
    }
}