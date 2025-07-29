/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:31:07 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/29 14:02:55 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static bool	match_loop(const char **item, const char **pat,
	const char **star, const char **match)
{
	if (**item == **pat)
	{
		(*item)++;
		(*pat)++;
	}
	else if (**pat == '*')
	{
		*star = (*pat)++;
		*match = *item;
	}
	else if (*star)
	{
		*pat = *star + 1;
		(*match)++;
		*item = *match;
	}
	else
		return (false);
	return (true);
}

bool	copy_old_args(char **new_list, char **old_list)
{
	int	i;

	i = 0;
	while (old_list[i])
	{
		new_list[i] = ft_strdup(old_list[i]);
		if (!new_list[i])
		{
			while (--i >= 0)
				free(new_list[i]);
			free(new_list);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	is_match(const char *item_name, const char *pattern)
{
	const char	*item = item_name;
	const char	*pat = pattern;
	const char	*star = NULL;
	const char	*match = NULL;

	while (*item)
		if (!match_loop(&item, &pat, &star, &match))
			return (false);
	while (*pat == '*')
		pat++;
	return (*pat == '\0');
}

int	count_lines(char	**lines)
{
	int	i;

	if (!lines)
		return (0);
	i = 0;
	while (lines[i])
		++i;
	return (i);
}

bool	find_directory_and_word(const char *line, char **dir, char **word)
{
	char	*last_backslash;

	last_backslash = ft_strrchr(line, '/');
	if (!last_backslash)
	{
		*dir = ft_strdup(".");
		*word = ft_strdup(line);
	}
	else
	{
		*dir = ft_substr(line, 0, last_backslash - line);
		*word = ft_substr(line, last_backslash - line + 1,
				ft_strlen(last_backslash + 1));
	}
	if (!*dir || !*word)
	{
		free(*dir);
		free(*word);
		return (false);
	}
	return (true);
}
