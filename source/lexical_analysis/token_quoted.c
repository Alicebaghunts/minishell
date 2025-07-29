/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 01:28:36 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 12:40:50 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	get_word_len_with_quotes(const char *line)
{
	int	i;
	int	single_quotes;
	int	double_quotes;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	while (line[i])
	{
		if (!single_quotes && !double_quotes && is_space(line[i]))
			break ;
		if (!single_quotes && !double_quotes && is_special_char(line[i]))
			break ;
		if (line[i] == '\'' && !double_quotes)
			single_quotes = !single_quotes;
		else if (line[i] == '"' && !single_quotes)
			double_quotes = !double_quotes;
		if (line[i] == '$' && !single_quotes)
		{
			i = skip_variable(line, i);
			continue ;
		}
		i++;
	}
	return (i);
}

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
	shell->exit_code = 0;
	shell->envp = envp;
	shell->tokens = NULL;
	shell->pwd = getcwd(NULL, 0);
	if (!shell->pwd)
	{
		shell->pwd = ft_strdup(".");
	}
	shell->history.fd = open(".mini_history", O_CREAT
			| O_RDWR | O_APPEND, 0644);
	if (shell->history.fd < 0)
		return (free(shell), NULL);
	shell->history.is_there_heredoc = false;
	return (shell);
}
