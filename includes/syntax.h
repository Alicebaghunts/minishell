/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisharu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:24:51 by mansargs          #+#    #+#             */
/*   Updated: 2025/07/29 14:02:10 by alisharu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

# include "token.h"

# include <sys/wait.h>

# define STRICT 1
# define NOT_STRICT 0

t_token	*last_token(t_token *head);
void	manage_parenthesis(const t_token *token, int *opened_parenthesis);
bool	should_I_wait(const t_token *last_token);
bool	syntax_and_heredoc(t_shell *shell);
bool	wait_for_input(t_shell *shell, char **line);
bool	invalid_redirect(const t_token *token, const int strict_flag);
bool	invalid_operator(const t_token *token, const int strict_flag);
bool	strict_syntax_errors(t_shell *shell);
bool	valid_line(t_shell *shell, char **line);
bool	secondary_syntax_errors(const t_token *token, int *opened_parenthesis);
int		handle_quots(char **envp, t_token *token);
char	*open_heredoc(const t_token *tokens, const int fd_history);
int		is_env_char(char c);
char	*get_env_value(char **envp, const char *str);
char	*ft_strjoin_free(char *s1, const char *s2);

bool	is_quoted_delimiter(const char *s);
char	*open_quotes_heredoc(const char *str, int *open_flag);
char	*open_quotes(char **envp, const char *str, int *open_flag);
void	append_char(char **res, char c);
int		check_is_open_quote(int quote, char *res);
char	*get_file_name(void);
int		get_random_number(void);

#endif
