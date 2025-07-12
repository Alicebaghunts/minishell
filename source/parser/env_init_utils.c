/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:47:10 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/12 15:47:12 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	env_size(t_env *env)
{
	int			i;
	int			total;
	t_env_node	*node;

	total = 0;
	i = 0;
	while (i < HASH_SIZE)
	{
		node = env->env[i];
		while (node)
		{
			total++;
			node = node->next;
		}
		i++;
	}
	return (total);
}

t_env_node	*env_get(t_env *env, const char *key)
{
	t_env_node	*node;
	int			index;

	index = hash_key(key);
	node = env->env[index];
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
			return (node);
		node = node->next;
	}
	return (NULL);
}

char	*get_value_data(char *env_line)
{
	char	*equal_pos;

	equal_pos = ft_strchr(env_line, '=');
	if (!equal_pos)
		return (NULL);
	return (ft_strdup(equal_pos + 1));
}

char	*get_key_data(char *env_line)
{
	char	*equal_pos;

	equal_pos = ft_strchr(env_line, '=');
	if (!equal_pos)
		return (ft_strdup(env_line));
	return (ft_substr(env_line, 0, equal_pos - env_line));
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
