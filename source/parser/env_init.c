/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:37:31 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/12 15:44:41 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_env_node	*create_env_node(char *key, char *value, int is_equal)
{
	t_env_node	*new;

	new = malloc(sizeof(t_env_node));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->is_equal = is_equal;
	new->next = NULL;
	return (new);
}

void	compair_key(t_env_node *current, char *key, char *value, int is_equal)
{
	if (ft_strcmp(current->key, key) == 0)
	{
		if (current->value)
			free(current->value);
		if (value)
			current->value = ft_strdup(value);
		else
			current->value = NULL;
		current->is_equal = is_equal;
		return ;
	}
}

void	env_set(t_env *env, char *key, char *value, int is_equal)
{
	unsigned int	index;
	t_env_node		*current;
	t_env_node		*new_node;

	index = hash_key(key);
	current = env->env[index];
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
	{
		compair_key(current, key, value, is_equal);
		return ;
	}
		current = current->next;
	}
	new_node = create_env_node(key, value, is_equal);
	if (!new_node)
		return ;
	new_node->next = env->env[index];
	env->env[index] = new_node;
}

void	env_init_from_envp(t_env *env, char **envp)
{
	int		i;
	char	*key;
	char	*value;
	int		has_equal;

	i = 0;
	while (envp[i])
	{
		key = get_key_data(envp[i]);
		value = get_value_data(envp[i]);
		has_equal = (ft_strchr(envp[i], '=') != NULL);
		env_set(env, key, value, has_equal);
		free(key);
		free(value);
		i++;
	}
}

t_env	*init_env(char **envp)
{
	t_env	*environment;
	int		i;

	environment = malloc(sizeof(t_env));
	if (!environment)
		return (NULL);
	i = 0;
	while (i < HASH_SIZE)
		environment->env[i++] = NULL;
	env_init_from_envp(environment, envp);
	return (environment);
}
