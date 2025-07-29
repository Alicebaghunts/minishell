/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:22:52 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 13:23:29 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static bool	open_input_redirect(t_token *redirect)
{
	int	fd;

	fd = open(redirect->file_name, O_RDONLY);
	if (fd < 0)
		return (perror(redirect->file_name), false);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (true);
}

static bool	open_output_redirect(t_token *redirect)
{
	int	fd;

	if (redirect->token_redirect_type == REDIRECT_OUT)
		fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(redirect->file_name), false);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (true);
}

bool	open_redirects(t_ast *node)
{
	t_token	*redirect;

	if (!node->redir)
		return (true);
	redirect = node->redir;
	while (redirect)
	{
		if (redirect->token_redirect_type == REDIRECT_HEREDOC
			|| redirect->token_redirect_type == REDIRECT_IN)
		{
			if (!open_input_redirect(redirect))
				return (false);
		}
		else
		{
			if (!open_output_redirect(redirect))
				return (false);
		}
		redirect = redirect->next_token;
	}
	return (true);
}
