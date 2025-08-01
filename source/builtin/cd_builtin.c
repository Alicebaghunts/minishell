/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mansargs <mansargs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:27:17 by alisharu          #+#    #+#             */
/*   Updated: 2025/08/02 02:50:46 by mansargs         ###   ########.fr       */
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

t_builtin_status	cd_builtin(char **args, t_env *env)
{
	char		*old_pwd;
	char		new_pwd[PATH_MAX];
	char		*path;
	t_env_node	*pwd_node;

	path = cd_validation(args, env);
	if (path == NULL)
		return (BUILTIN_FAIL);
	old_pwd = env_get(env, "PWD")->value;
	if (chdir(path) != 0)
	{
		env->shell->exit_code = 1;
		perror("minishell: cd");
		return (BUILTIN_FAIL);
	}
	if (!getcwd(new_pwd, sizeof(new_pwd)))
	{
		printf("cd: error retrieving current directory: getcwd: ");
		printf("cannot access parent directories: No such file or directory\n");
		pwd_node = env_get(env, "PWD");
		if (!pwd_node || !pwd_node->value)
			env->shell->pwd = ft_strdup("");
		else
			env->shell->pwd = ft_strjoin(pwd_node->value, "/..");
		env_set(env, "PWD", env->shell->pwd, 1);
		return (BUILTIN_FAIL);
	}
	env_set(env, "OLDPWD", old_pwd, 1);
	env_set(env, "PWD", new_pwd, 1);
	free(env->shell->pwd);
	env->shell->pwd = ft_strdup(new_pwd);
	return (BUILTIN_OK);
}
