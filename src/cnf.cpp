#include <cnf_utils.hpp>

namespace pll::cnf
{
    using pll::detail::bst_node;

    std::string prop_to_cnf(token_stream& stream, 
                            const connective_prop_map& conn_props)
    {
        std::string result;

        auto root = detail::simplify_cnf_expr(stream, conn_props);
        detail::bst_to_expr(root, result, conn_props);

        pll::detail::bst_release(root);
        return result;
    }
}   