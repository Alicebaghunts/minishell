/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 19:49:43 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/29 13:24:10 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	restore_standard_fd(int std_in, int std_out)
{
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	close(std_in);
	close(std_out);
}

int	execute_logic_and(t_ast *node, t_env *env)
{
	int	left_return;

	left_return = execute_ast(node->left_side, env, false);
	if (left_return != 0)
		return (left_return);
	return (execute_ast(node->right_side, env, false));
}

int	execute_logic_or(t_ast *node, t_env *env)
{
	int	left_return;

	left_return = execute_ast(node->left_side, env, false);
	if (left_return == 0)
		return (0);
	return (execute_ast(node->right_side, env, false));
}

int	execute_ast(t_ast *node, t_env *env, bool has_forked)
{
	int	old_stdin;
	int	old_stdout;
	int	result;

	if (!node || !node->cmd)
		return (0);
	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	if (!open_redirects(node))
		return (restore_standard_fd(old_stdin, old_stdout), 1);
	if (node->cmd->token_operator_type == OPERATOR_AND)
		result = execute_logic_and(node, env);
	else if (node->cmd->token_operator_type == OPERATOR_OR)
		result = execute_logic_or(node, env);
	else if (node->cmd->token_operator_type == OPERATOR_PIPE)
		result = execute_pipe(node, env);
	else if (node->cmd->token_paren_type == PAREN_OPEN)
		result = execute_subshell(node, env, has_forked);
	else
		result = execute_command(node, env, has_forked);
	restore_standard_fd(old_stdin, old_stdout);
	if (result == -1)
		return (-1);
	return (result);
}
