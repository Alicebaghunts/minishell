/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:54:15 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 12:40:25 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

bool	is_str_greater(const char *str1, const char *str2)
{
	int	len1;
	int	len2;
	int	i;

	i = 0;
	len1 = strlen(str1);
	len2 = strlen(str2);
	if (len1 > len2)
		return (true);
	if (len1 < len2)
		return (false);
	while (i < len1)
	{
		if (str1[i] > str2[i])
			return (true);
		if (str1[i] < str2[i])
			return (false);
		i++;
	}
	return (false);
}

bool	ft_str_is_too_big(const char *str)
{
	int		i;
	bool	negative;

	negative = false;
	i = 0;
	if (!str || str[0] == '\0')
		return (true);
	if (str[0] == '-')
	{
		negative = true;
		i++;
	}
	else if (str[0] == '+')
		i++;
	while (str[i] == '0')
		i++;
	if (str[i] == '\0')
		return (false);
	if (negative)
		if (is_str_greater(&str[i], LONG_MIN_STR))
			return (true);
	if (!negative)
		if (is_str_greater(&str[i], LONG_MAX_STR))
			return (true);
	return (false);
}

bool	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (false);
	if (str[0] == '+' || str[0] == '-')
		i++;
	if (str[i] == '\0')
		return (false);
	while (str[i])
	{
		if (!isdigit((unsigned char)str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	exit_builtin(t_shell *shell, char **args)
{
	long	exit_code;

	exit_code = 0;
	ft_putendl_fd("exit", 1);
	if (!args[1])
		exit(shell->exit_code);
	if (!is_numeric(args[1]) || ft_str_is_too_big(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(255);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		shell->exit_code = 1;
		return ;
	}
	exit_code = ft_atol(args[1]);
	exit(exit_code % 256);
}
