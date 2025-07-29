/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 02:47:43 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/29 13:59:47 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

static void	read_from_stdin(const int fd, const char *delim,
	const int fd_history)
{
	char	*line;
	size_t	len;
	int		count_lines;

	count_lines = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: warning: here-document at line %d "
				"delimited by EOF (wanted `%s')\n", count_lines, delim);
			return ;
		}
		++count_lines;
		ft_putendl_fd(line, fd_history);
		len = ft_strlen(line);
		if (len == ft_strlen(delim) && ft_strncmp(line, delim, len) == 0)
		{
			free(line);
			return ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static bool	is_invalid_token(const t_token *token)
{
	if (token->file_name)
		return (true);
	if (!token->next_token)
	{
		printf("%s `newline'\n", SYN_ERR);
		return (true);
	}
	return (false);
}

static void	heredoc_child(int fd, const t_token *token, int fd_history)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	read_from_stdin(fd, token->next_token->token_data, fd_history);
	close(fd);
	exit(0);
}

static int	fork_and_wait_heredoc(int fd, const t_token *token, int fd_history)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (-1);
	}
	if (pid == 0)
		heredoc_child(fd, token, fd_history);
	if (waitpid(pid, &status, 0) == -1 || WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		g_received_signal = WTERMSIG(status);
		return (-1);
	}
	return (0);
}

char	*open_heredoc(const t_token *token, const int fd_history)
{
	char	*name;
	int		fd;

	if (is_invalid_token(token))
		return (NULL);
	name = get_file_name();
	if (!name)
		return (NULL);
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (free(name), NULL);
	if (fork_and_wait_heredoc(fd, token, fd_history) == -1)
		return (free(name), NULL);
	return (name);
}
