/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:29:32 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 15:52:36 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	pwd_builtin(char **args, t_env *env)
{
	// char	*cwd;

	(void)args;
	// if (!env->shell->pwd)
	// {
	// 	cwd = getcwd(NULL, 0);
	// 	if (!cwd)
	// 	{
	// 		perror("minishell: pwd");
	// 		env->shell->exit_code = 1;
	// 		return ;
	// 	}
	// 	env->shell->pwd = cwd;
	// }
	printf("%s\n", env->shell->pwd);
	env->shell->exit_code = 0;
}
