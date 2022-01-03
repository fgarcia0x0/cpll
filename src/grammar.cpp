#include <token.hpp>
#include <token_stream.h>
#include <utils.hpp>
#include <cnf.hpp>

using token_type = pll::token::token_type;

namespace pll::grammar::detail
{
	static inline bool expect(token_type type, token_stream& stream)
	{
		if (stream.current_token()->type == type)
			stream.next_token();
		else
			return false;
				
		return true;
	}

	static bool recognize_term(token_stream&);

	static bool recognize_expr(token_stream& stream)
	{
		if (!recognize_term(stream))
			return false;

		while (!stream.eos() && is_binary_op(stream.current_token().value(), stream))
		{
			stream.next_token();
			if (!recognize_term(stream))
				return false;
		}
			
		return true;
	}

	static bool recognize_term(token_stream& stream)
	{
		std::optional<token> curr = stream.current_token();
			
		if (!curr.has_value())
			return false;
			
		if (curr->type == token::type::atom)
		{
			stream.next_token();
			return true;
		}
		else if (curr->type == token::type::lparan)
		{
			stream.next_token();

			if (!recognize_expr(stream))
				return false;
							
			if (!expect(token::type::rparan, stream))
				return false;
		}
		else if (is_unary_op(curr.value(), stream))
		{
			stream.next_token();
			if (!recognize_term(stream))
				return false;
		}
		else
			return false;

		return true;
	}
}

namespace pll::grammar
{
	/**
	 * @brief An Recursive Descent Recognizer
	 * 
	 * @param stream a stream of tokens
	 * @return true if grammar composed of the tokens of stream is valid, otherwise false.
	 */
	bool recognize(token_stream& stream)
	{
		if (stream.eos())
			throw std::invalid_argument{ "invalid token stream" };
		else
			return detail::recognize_expr(stream) && stream.eos();
	}
}