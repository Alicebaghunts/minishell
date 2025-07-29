/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:27:17 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 15:52:13 by alisharu         ###   ########.fr       */
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

char	*cd_validation(char **args, t_env *env)
{
	char	*path;

	if (!check_too_many_dirs(args, env))
		return (NULL);
	if (!args[1])
	{
		path = handle_cd_without_dir(args, env);
		if (!path)
			return (NULL);
	}
	else
		path = args[1];
	return (path);
}

void	cd_builtin(char **args, t_env *env)
{
	char	*old_pwd;
	char	new_pwd[PATH_MAX];
	char	*path;

	path = cd_validation(args, env);
	if (path == NULL)
		return ;
	old_pwd = env_get(env, "PWD")->value;
	if (chdir(path) != 0)
	{
		env->shell->exit_code = 1;
		perror("minishell: cd");
		return ;
	}
	if (!getcwd(new_pwd, sizeof(new_pwd)))
	{
		printf("cd: error retrieving current directory: getcwd: ");
		printf("cannot access parent directories: No such file or directory\n");
		env->shell->pwd = ft_strjoin(env_get(env, "PWD")->value, "/..");
		env_set(env, "PWD", env->shell->pwd, 1);
		return ;
	}
	env_set(env, "OLDPWD", old_pwd, 1);
	env_set(env, "PWD", new_pwd, 1);
}
