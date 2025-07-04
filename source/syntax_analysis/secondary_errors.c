/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secondary_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 03:03:48 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/04 13:21:10 by alisharu         ###   ########.fr       */
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
	if (token->token_paren_type == PAREN_OPEN
		&& invalid_operator(token, NOT_STRICT))
		return (true);
	if (!token->next_token && *opened_parenthesis > 0)
		return (printf("%s `('\n", SYN_ERR), true);
	return (false);
}
