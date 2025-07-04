/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secondary_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mansargs <mansargs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 03:03:48 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/04 15:08:09 by mansargs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

void	manage_parenthesis(const t_token *token, int *opened_parenthesis)
{
	if (token->token_paren_type == PAREN_OPEN)
		++*opened_parenthesis;
	if (token->token_paren_type == PAREN_CLOSE)
		--*opened_parenthesis;
}

static bool invalid_parenthesis(const t_token *token)
{
	if (token->next_token && (token->next_token->token_paren_type == PAREN_CLOSE
		|| token->next_token->token_type == TOKEN_OPERATOR))
		return (printf("%s `%s'\n", SYN_ERR, token->next_token->token_data), true);
	return (false);
}

bool	secondary_syntax_errors(const t_token *token, int	*opened_parenthesis)
{
	manage_parenthesis(token, opened_parenthesis);
	if (token->token_type == TOKEN_REDIRECT
		&& invalid_redirect(token, NOT_STRICT))
		return (true);
	if (token->token_paren_type == PAREN_CLOSE && *opened_parenthesis < 0)
		return (printf("%s `)'\n", SYN_ERR), true);
	if (token->token_type == TOKEN_WORD && invalid_word(token))
		return (true);
	if (token->token_paren_type == PAREN_OPEN && invalid_parenthesis(token))
		return (true);
	if (!token->next_token && *opened_parenthesis > 0)
		return (printf("%s `newline'\n", SYN_ERR), true);
	return (false);
}
