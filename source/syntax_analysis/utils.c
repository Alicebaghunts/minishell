/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:19:44 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 12:50:14 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	is_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*get_env_value(char **envp, const char *str)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], str, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
}

void	free_matrix(char ***matrix)
{
	int	i;

	if (!matrix || !*matrix)
		return ;
	i = 0;
	while ((*matrix)[i])
		free((*matrix)[i++]);
	free(*matrix);
	*matrix = NULL;
}

void	free_ast(t_ast *tree)
{
	if (!tree)
		return ;
	if (tree->left_side)
		free_ast(tree->left_side);
	if (tree->right_side)
		free_ast(tree->right_side);
	free(tree);
}
