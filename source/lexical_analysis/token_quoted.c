/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 01:28:36 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/04 13:56:17 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

static int	add_quoted_token(char *line, int start, t_token **head, int j)
{
	char	*substr;
	t_token	*new_token;

	substr = ft_substr(line, start, j - start);
	if (!substr)
		return (-1);
	new_token = create_token(substr, TOKEN_WORD);
	free(substr);
	if (!new_token)
		return (-1);
	add_token(head, new_token);
	return (0);
}

int	handle_quots_token(char *line, int i, t_token **head)
{
	int		start;
	int		j;
	char	quote;

	start = i;
	j = i;
	while (line[j])
	{
		if (line[j] == '\'' || line[j] == '"')
		{
			quote = line[j++];
			while (line[j] && line[j] != quote)
				j++;
			if (line[j] == quote)
				j++;
		}
		else if (!is_space(line[j]) && !is_special_char(line[j]))
			j++;
		else
			break ;
	}
	if (add_quoted_token(line, start, head, j) < 0)
		return (-1);
	return (j - i);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (shell == NULL)
		return (NULL);
	shell->envp = envp;
	shell->tokens = NULL;
	return (shell);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next_token;
		free(tmp->token_data);
		free(tmp);
	}
}
