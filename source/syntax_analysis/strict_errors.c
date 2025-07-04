/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strict_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 02:55:20 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/04 14:30:57 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

bool	invalid_word(const t_token *token)// es em avelacrel
{
	if (token->next_token && token->next_token->token_paren_type == PAREN_OPEN)
		return (printf("%s `('\n", SYN_ERR), true);
	return (false);
}

bool	invalid_redirect(const t_token *token, const int strict_flag)
{
	if (strict_flag)
	{
		if (token->next_token && token->next_token->token_type
			== TOKEN_REDIRECT)
			return (printf("%s `%s'\n", SYN_ERR,
					token->next_token->token_data), true);// >> >
		if (token->next_token && (token->next_token->token_type == TOKEN_OPERATOR
			|| token->next_token->token_paren_type == PAREN_OPEN))// >> &&, > (
			return (printf("%s `%s'\n", SYN_ERR,
					token->next_token->token_data), true);
	}
	else
	{
		if (!token->next_token)
			return (printf("%s `newline'\n", SYN_ERR), true);// ls <<
		if (token->next_token && token->next_token->token_paren_type == PAREN_CLOSE)// >> (n
			return (printf("%s `)'\n", SYN_ERR), true);
	}
	return (false);
}

bool	invalid_operator(const t_token *token, const int strict_flag)
{
	if (strict_flag)
	{
		if (!token->prev_token && token->token_type == TOKEN_OPERATOR)
			return (printf("%s `%s'\n", SYN_ERR, token->token_data), true);
		if (token->next_token
			&& token->next_token->token_type == TOKEN_OPERATOR)
			return (printf("%s `%s'\n", SYN_ERR, token->next_token->token_data),
				true);
		if (token->next_token && token->next_token->token_type == TOKEN_OPERATOR)
			return (printf("%s `%s'\n", SYN_ERR,
					token->next_token->token_data), true);
	}
	else
	{
		if (token->next_token && token->next_token->token_paren_type == PAREN_CLOSE)
			return (printf("%s `%s'\n", SYN_ERR, token->next_token->token_data),
				true);
	}
	return (false);
}

bool	strict_syntax_errors(t_shell *shell)
{
	t_token	*temp;

	temp = shell->tokens;
	while (temp)
	{
		if (temp->token_type == TOKEN_REDIRECT)
			if (invalid_redirect(temp, STRICT))
				return (true);
		if (temp->token_type == TOKEN_OPERATOR)
			if (invalid_operator(temp, STRICT))
				return (true);
		if (temp->token_type == TOKEN_WORD)
		{// es masy
		
			if (handle_quots(shell->envp, temp) == -1)
				return (true);
		}
		temp = temp->next_token;
	}
	return (false);
}
