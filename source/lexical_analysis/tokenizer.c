/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mansargs <mansargs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:35:41 by alisharu          #+#    #+#             */
/*   Updated: 2025/06/21 18:34:47 by mansargs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*tmp;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	tmp = *head;
	while (tmp->next_token)
		tmp = tmp->next_token;
	tmp->next_token = new_token;
}

static int	get_word_len_with_quotes(const char *line)
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

static int	handle_operator_token(const char *line, int i, t_token **head)
{
	t_token_type	type;
	t_token			*tok;
	char			*substr;
	int				len;

	len = 1;
	if ((line[i] == '&' && line[i + 1] == '&')
		|| (line[i] == '|' && line[i + 1] == '|')
		|| (line[i] == '<' && line[i + 1] == '<')
		|| (line[i] == '>' && line[i + 1] == '>'))
		len = 2;
	type = get_token_type(&line[i], len);
	substr = ft_substr(&line[i], 0, len);
	if (!substr)
		return (-1);
	tok = create_token(substr, type);
	free(substr);
	if (!tok)
		return (-1);
	add_token(head, tok);
	return (len);
}

static int	handle_word_token(const char *line, int i, t_token **head)
{
	t_token	*tok;
	char	*substr;
	int		len;

	len = get_word_len_with_quotes(&line[i]);
	substr = ft_substr(&line[i], 0, len);
	if (!substr)
		return (-1);
	tok = create_token(substr, TOKEN_WORD);
	free(substr);
	if (!tok)
		return (-1);
	add_token(head, tok);
	return (len);
}

t_token	*tokenize(char *line)
{
	t_token	*head;
	int		len;
	int		i;

	i = 0;
	head = NULL;
	while (line[i])
	{
		if (is_space(line[i]))
		{
			++i;
			continue ;
		}
		else if (is_special_char(line[i]))
			len = handle_operator_token(line, i, &head);
		else if (line[i] == '\'' || line[i] == '"')
			len = handle_quoted_token(line, i, &head);
		else
			len = handle_word_token(line, i, &head);
		if (len < 0)
			return (free_tokens(head), NULL);
		i += len;
	}
	return (head);
}
