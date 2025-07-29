/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absent_operand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 00:51:12 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/29 15:23:35 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

static bool	read_and_join_line(char **line, char **extra_line)
{
	char	*temp;

	while (1)
	{
		*extra_line = readline("> ");
		if (!*extra_line)
			return (false);
		if (only_spaces(*extra_line))
		{
			free(*extra_line);
			continue ;
		}
		if (!*line)
			*line = ft_strdup(*extra_line);
		else
		{
			temp = *line;
			*line = ft_strjoin(*line, *extra_line);
			free(temp);
		}
		if (!*line)
			return (free(*extra_line), false);
		break ;
	}
	return (true);
}

static int	validate_and_merge_tokens(t_shell *shell, char **extra_line)
{
	t_token	*new_tokens;
	t_token	*prev;
	t_token	*last;

	new_tokens = tokenize(*extra_line);
	free(*extra_line);
	*extra_line = NULL;
	if (!new_tokens)
		return (1);
	prev = shell->tokens;
	shell->tokens = new_tokens;
	if (!syntax_and_heredoc(shell))
		return (2);
	last = add_token(&prev, shell->tokens);
	shell->tokens = prev;
	if (last->token_type == TOKEN_OPERATOR)
		return (3);
	return (0);
}

bool	wait_for_input(t_shell *shell, char **line)
{
	char	*extra_line;
	int		result;

	while (1)
	{
		if (!read_and_join_line(line, &extra_line))
			return (false);
		result = validate_and_merge_tokens(shell, &extra_line);
		if (result == 3)
			continue ;
		else if (result == 1)
			return (false);
		break ;
	}
	return (true);
}
