/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:02:10 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 13:06:13 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

bool	collect_matched(char ***matched, const char *newly_added)
{
	char	**new_list;
	int		len;

	if (!*newly_added)
		return (true);
	len = count_lines(*matched);
	new_list = ft_calloc(len + 2, sizeof(char *));
	if (!new_list)
		return (false);
	if (*matched && !copy_old_args(new_list, *matched))
		return (false);
	new_list[len] = ft_strdup(newly_added);
	if (!new_list[len])
		return (free_matrix(&new_list), false);
	new_list[len + 1] = NULL;
	free_matrix(matched);
	*matched = new_list;
	return (true);
}

bool	open_directory(const char *dir, const char *word, char ***incoming_arg)
{
	DIR				*dir_content;
	struct dirent	*item;

	*incoming_arg = NULL;
	dir_content = opendir(dir);
	if (!dir_content)
		return (true);
	while (1)
	{
		item = readdir(dir_content);
		if (!item)
			break ;
		if (!ft_strcmp(".", item->d_name) || !ft_strcmp("..", item->d_name))
			continue ;
		if (is_match(item->d_name, word))
			if (!collect_matched(incoming_arg, item->d_name))
				return (free_matrix(incoming_arg),
					closedir(dir_content), false);
	}
	closedir(dir_content);
	return (true);
}

bool	copy_and_check(char **dst, char **src, int *i)
{
	int	j;

	j = 0;
	while (src[j])
	{
		dst[*i] = ft_strdup(src[j]);
		if (!dst[*i])
			return (false);
		(*i)++;
		j++;
	}
	return (true);
}

bool	integration(char ***argv, char **replace, int idx)
{
	char	**new_argv;
	int		i;

	if (!replace)
		return (true);
	new_argv = ft_calloc(count_lines(*argv) + count_lines(replace) + 1,
			sizeof(char *));
	if (!new_argv)
		return (false);
	i = 0;
	while (i < idx)
	{
		new_argv[i] = ft_strdup((*argv)[i]);
		if (!new_argv[i++])
			return (free_matrix(&new_argv), false);
	}
	if (!copy_and_check(new_argv, replace, &i))
		return (free_matrix(&new_argv), false);
	while ((*argv)[++idx])
	{
		new_argv[i] = ft_strdup((*argv)[idx]);
		if (!new_argv[i++])
			return (free_matrix(&new_argv), false);
	}
	return (free_matrix(argv), free_matrix(&replace), *argv = new_argv, true);
}

bool	open_wildcards(char	***argv)
{
	int		i;
	char	*dir;
	char	*word;
	char	**replace;

	i = -1;
	while ((*argv)[++i])
	{
		if (!find_directory_and_word((*argv)[i], &dir, &word))
			return (false);
		if (!open_directory(dir, word, &replace))
			return (free(word), free(dir), false);
		free(word);
		free(dir);
		if (!integration(argv, replace, i))
			return (free_matrix(&replace), false);
	}
	return (true);
}
