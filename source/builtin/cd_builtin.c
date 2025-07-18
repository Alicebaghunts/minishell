/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:27:17 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/13 12:53:29 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*handle_cd_without_dir(char **args, t_env *env)
{
	char		*path;
	t_env_node	*home;

	path = NULL;
	if (!args[1])
	{
		home = env_get(env, "HOME");
		if (!home || !home->value)
		{
			printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
		path = home->value;
	}
	return (path);
}

int	check_too_many_dirs(char **args, t_env *env)
{
	int	count;

	(void)env;
	count = 0;
	while (args && args[count])
		count++;
	if (count > 2)
	{
		printf("minishell: cd: too many arguments\n");
		return (0);
	}
	return (1);
}

void	cd_builtin(char **args, t_env *env)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];
	char	*path;

	if (!check_too_many_dirs(args, env))
		return ;
	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		perror("minishell: cd: getcwd");
		return ;
	}
	if (!args[1])
	{
		path = handle_cd_without_dir(args, env);
		if (!path)
			return ;
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return ;
	}
	if (!getcwd(new_pwd, sizeof(new_pwd)))
	{
		perror("minishell: cd: error retrieving current directory");
		return ;
	}
	env_set(env, "OLDPWD", old_pwd, 1);
	env_set(env, "PWD", new_pwd, 1);
}
