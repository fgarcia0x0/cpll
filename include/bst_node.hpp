#pragma once

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

        /* Enable Default Copy */
        constexpr bst_node(const bst_node&) = default;
        constexpr bst_node& operator = (const bst_node&) = default;

        constexpr bst_node(const token token, bst_node* lhs, bst_node* rhs)
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
}