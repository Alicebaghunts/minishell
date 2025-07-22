/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:11:25 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/21 20:55:35 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	print_list(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->tokens;
	while (tmp != NULL)
	{
		printf("%s -> ", tmp->token_data);
		tmp = tmp -> next_token;
	}
	printf("NULL\n");
}

bool	only_spaces(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!is_space(str[i]))
			return (false);
	}
	return (true);
}

static void print_indent(int level)
{
	for (int i = 0; i < level; ++i)
		printf("│   ");
}

static void print_token_chain(t_token *tok)
{
	while (tok)
	{
		printf("%s", tok->token_data);
        if (tok->file_name)
            printf("%s", tok->file_name);
		if (tok->next_token)
			printf(" ");
		tok = tok->next_token;
	}
}

static void print_redir_chain(t_token *redir)
{
	if (!redir)
		return;
	printf(" [ ");
	print_token_chain(redir);
	printf(" ]");
}

void print_ast_full(t_ast *node, int level)
{
	print_indent(level);
	if (node->cmd && node->cmd->token_paren_type == PAREN_OPEN)
	{
		// Subshell detected
		printf("● Subshell: ( ) ");
		print_redir_chain(node->redir);
		printf("\n");

		print_indent(level + 1);
		printf("└─ Subshell Body:\n");
		print_ast_full(node->left_side, level + 2);

		// ❌ DO NOT print left_side and right_side again
		return;
	}

	if (node->cmd && node->cmd->token_type == TOKEN_OPERATOR)
	{
		printf("● Operator: %s ", node->cmd->token_data);
	}
	else
	{
		printf("● Command: ");
		print_token_chain(node->cmd);
		print_redir_chain(node->redir);
	}
	printf("\n");

	if (node->left_side)
	{
		print_indent(level + 1);
		printf("├─ Left:\n");
		print_ast_full(node->left_side, level + 2);
	}
	if (node->right_side)
	{
		print_indent(level + 1);
		printf("└─ Right:\n");
		print_ast_full(node->right_side, level + 2);
	}
}


void	print_env_table(t_env *env)
{
	int				i;
	t_env_node		*node;

	i = 0;
	while (i < HASH_SIZE)
	{
		node = env->env[i];
		// if (node)
		// 	printf("Index %d:\n", i);
		while (node)
		{
			printf("  \033[34m%s\033[0m", node->key);
			if (node->is_equal)
			{
				printf("\033[37m=\033[0m");
				if (node->value)
					printf("\033[32m%s\033[0m", node->value);
			}
			printf("\n");
			node = node->next;
		}
		i++;
	}
}

int	main(int argc, char *argv[], char **envp)
{
	char	*line;
	t_shell	*shell;
	t_env	*my_env;
	t_ast	*tree;

	(void)argv;
	if (argc > 1)
	{
		printf("This program must be run without any arguments.\n");
		return (EXIT_FAILURE);
	}
	shell = init_shell(envp);
	my_env = init_env(shell, envp);
	if (!my_env)
	{
		printf("Failed to initialize env table.\n");
		return (1);
	}
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		shell->tokens = tokenize(line);
		if (!shell->tokens)
			continue ;
		add_history(line);
		if (shell->tokens && !valid_line(shell, &line))
		{
			free(line);
			continue ;
		}
		if (!(tree = building_ast(shell->tokens)))
			return (EXIT_FAILURE);
		// else
		// 	print_ast_full(tree,0);
		execute_ast(tree, my_env, 0);
		free(line);
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	free_env_table(my_env);
	if (shell)
		free(shell);
	shell = NULL;
	printf("exit\n");
	return (0);
}
