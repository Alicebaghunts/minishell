/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:04:36 by alisharu          #+#    #+#             */
/*   Updated: 2025/07/29 13:28:35 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

char	*open_quotes_heredoc(const char *str, int *open_flag)
{
	int		i;
	char	quote;
	char	*res;

	i = 0;
	quote = 0;
	*open_flag = 0;
	res = ft_calloc(1, 1);
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else
			append_char(&res, str[i]);
		i++;
	}
	if (check_is_open_quote(quote, res) == 0)
		return (NULL);
	return (res);
}

bool	is_quoted_delimiter(const char *s)
{
	size_t	len;

	if (!s)
		return (false);
	len = ft_strlen(s);
	if (len < 2)
		return (false);
	if ((s[0] == '\'' && s[len - 1] == '\'')
		|| (s[0] == '"' && s[len - 1] == '"'))
		return (true);
	return (false);
}

int	get_random_number(void)
{
	int				fd;
	unsigned int	number;
	int				result;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (-1);
	result = read(fd, &number, sizeof number);
	close(fd);
	if (result == -1)
		return (-1);
	return (number);
}

char	*get_file_name(void)
{
	char			*num_by_string;
	char			*file_name;
	unsigned int	number;

	while (1)
	{
		while (1)
		{
			number = get_random_number();
			if (number < INT_MAX)
				break ;
		}
		num_by_string = ft_itoa(number);
		if (!num_by_string)
			return (NULL);
		file_name = ft_strjoin(".heredoc_", num_by_string);
		free(num_by_string);
		if (access(file_name, F_OK) == -1)
			break ;
		free(file_name);
	}
	return (file_name);
}
