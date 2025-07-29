/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:16:24 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 13:21:14 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	print_exec_error(const char *cmd, int exit_code)
{
	if (strchr(cmd, '/'))
	{
		if (exit_code == 127)
			printf("minishell: %s: No such file or directory\n", cmd);
		else if (exit_code == 126)
			printf("minishell: %s: Permission denied\n", cmd);
	}
	else
		printf("%s: command not found\n", cmd);
}

int	prepare_command(t_ast *node, t_env *env, char ***argv)
{
	*argv = get_arguments(node->cmd, env);
	if (!*argv)
		return (-1);
	if (!open_wildcards(argv))
		return (free_matrix(argv), -1);
	if (execute_builtin(*argv, env))
	{
		free_matrix(argv);
		return (0);
	}
	return (1);
}

static pid_t	fork_child(int fd_in, int fd_out, t_ast *node, t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (fd_in != -1)
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		if (fd_out != -1)
		{
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		exit(execute_ast(node, env, true));
	}
	return (pid);
}

static int	wait_for_children(pid_t left, pid_t right)
{
	int	left_status;
	int	right_status;

	if (waitpid(left, &left_status, 0) == -1
		|| waitpid(right, &right_status, 0) == -1)
		return (perror("waiting failed"), 1);
	if (WIFSIGNALED(right_status))
	{
		if (WTERMSIG(right_status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (128 + WTERMSIG(right_status));
	}
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	return (0);
}

int	execute_pipe(t_ast *node, t_env *env)
{
	int		pipe_fds[2];
	pid_t	left;
	pid_t	right;

	if (pipe(pipe_fds) == -1)
		return (1);
	signal(SIGINT, SIG_IGN);
	left = fork_child(-1, pipe_fds[1], node->left_side, env);
	if (left == -1)
		return (1);
	right = fork_child(pipe_fds[0], -1, node->right_side, env);
	if (right == -1)
		return (1);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	return (wait_for_children(left, right));
}
