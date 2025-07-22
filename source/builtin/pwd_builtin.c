/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:29:32 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/21 21:22:46 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	pwd_builtin(t_shell *shell, t_env *env)
{
	char	*pwd;

	(void)shell;
	(void)env;
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		shell->pwd = pwd;
		printf("%s\n", pwd);
		free(pwd);
	}
	else
		printf("%s\n", shell->pwd);
}
