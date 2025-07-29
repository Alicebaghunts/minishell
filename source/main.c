/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:11:25 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 15:26:02 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

volatile sig_atomic_t	g_received_signal;

static bool	handle_line(t_shell *shell, t_env *my_env, char *line)
{
	t_ast	*tree;

	if (g_received_signal)
	{
		shell->exit_code = g_received_signal + 128;
		g_received_signal = 0;
	}
	shell->tokens = tokenize(line);
	if (!shell->tokens)
		return (false);
	add_history(line);
	if (!valid_line(shell, &line))
	{
		free(line);
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (false);
	}
	tree = building_ast(shell->tokens);
	if (!tree)
		return (false);
	execute_ast(tree, my_env, 0);
	free_ast(tree);
	free_tokens(shell->tokens);
	return (shell->tokens = NULL, true);
}

int	main(int argc, char *argv[], char **envp)
{
	char	*line;
	t_shell	*shell;
	t_env	*my_env;

	(void)argv;
	if (argc > 1)
		return (printf("This program must be run without any arguments.\n"),
			EXIT_FAILURE);
	shell = init_shell(envp);
	my_env = init_env(shell, envp);
	if (!my_env)
		return (printf("Failed to initialize env table.\n"), 1);
	while (1)
	{
		setup_signals();
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (!handle_line(shell, my_env, line))
			continue ;
		free(line);
	}
	free_env_table(my_env);
	free(shell);
	return (printf("exit\n"), 0);
}
