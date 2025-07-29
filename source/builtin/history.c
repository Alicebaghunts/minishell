/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 02:18:23 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/29 13:29:10 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	history_builtin(void)
{
	char			*line;
	unsigned long	count_line;
	int				fd;

	fd = open(".mini_history", O_RDONLY);
	if (fd < 0)
	{
		perror("history doesn't open");
		return ;
	}
	count_line = 0;
	while (1)
	{
		++count_line;
		line = get_next_line(fd);
		if (!line)
			return ;
		printf("%lu %s", count_line, line);
		free(line);
	}
	if (close(fd) == -1)
		perror("close history failed");
}
