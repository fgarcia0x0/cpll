#pragma once

#include <token_stream.h>
#include <cnf_utils.hpp>

namespace pll::cnf
{
    /**
     * @brief The function is basically responsible for receiving user entry
     *        as a correct proposition and transform into an CNF proposition
     * 
     * @param stream a stream of tokens
     * @param conn_props hashmap for connective properties
     * @return proposition CNF in string format
     *
     * @details
     *      @warning:
     *          - Do not place external parentheses at user entry
     *          - Put the parentheses in each subexpression
     *                      examples:
     *                          * a > b > c == ERROR
     *                          * (a > b) > c or a > (b > c) == CORRECT
     *                          * -(-(-a)) == CORRECT and -(--a) == ERROR
     */

    std::string prop_to_cnf(token_stream& stream, 
                            const connective_prop_map& conn_props);
}
