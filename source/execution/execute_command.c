/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:53:17 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 13:13:30 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <errno.h>
#include <unistd.h>

static int	child_execute(char **argv, t_env *env)
{
	char	*cmd_path;
	char	**envp;

	cmd_path = find_command_path(argv[0], env);
	if (!cmd_path)
	{
		print_exec_error(argv[0], env->shell->exit_code);
		exit(env->shell->exit_code);
	}
	envp = convert_env_to_matrix(env);
	execve(cmd_path, argv, envp);
	perror("execve");
	free(cmd_path);
	free_matrix(&envp);
	exit(126);
}

static int	execute_command_no_fork(t_ast *node, t_env *env)
{
	char	**argv;

	argv = get_arguments(node->cmd, env);
	if (!argv)
		return (-1);
	if (!open_wildcards(&argv))
		return (free(argv), -1);
	if (execute_builtin(argv, env))
	{
		free_matrix(&argv);
		return (0);
	}
	child_execute(argv, env);
	exit(EXIT_FAILURE);
}

static int	fork_and_wait(char **argv, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (perror("fork failed"), free_matrix(&argv), -1);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		child_execute(argv, env);
	}
	waitpid(pid, &status, 0);
	free_matrix(&argv);
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

static int	execute_command_with_fork(t_ast *node, t_env *env)
{
	char	**argv;
	int		res;

	res = prepare_command(node, env, &argv);
	if (res != 1)
		return (res);
	return (fork_and_wait(argv, env));
}

int	execute_command(t_ast *node, t_env *env, bool has_forked)
{
	if (has_forked)
		return (execute_command_no_fork(node, env));
	else
		return (execute_command_with_fork(node, env));
}
