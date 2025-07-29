/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:20:08 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 13:20:19 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	fork_and_execute_subshell(t_ast *node, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (perror("fork failed"), -1);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exit(execute_ast(node->left_side, env, true));
	}
	if (waitpid(pid, &status, 0) == -1)
		return (perror("waiting failed"), -1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (WTERMSIG(status) + 128);
	}
	return (1);
}

int	execute_subshell(t_ast *node, t_env *env, bool has_forked)
{
	int	result;

	if (has_forked)
		return (execute_ast(node->left_side, env, true));
	result = fork_and_execute_subshell(node, env);
	env->shell->exit_code = result;
	return (result);
}
