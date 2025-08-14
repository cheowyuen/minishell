/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:32:43 by cwong             #+#    #+#             */
/*   Updated: 2025/08/12 11:36:30 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

int		parse_input(char *input, t_token **tokens);
int		free_tokens_input(t_token **tokens, char *input);
int		add_token(t_token **tokens, t_quote_type quote_type, char *content, int len);
int		is_special(char c);
int		is_quote(char c);
int		is_space(char c);
void	ft_lstclear(t_token **lst);
char	*ft_strndup(const char *src, int n);
void	ft_lstadd_back(t_token **lst, t_token *new);
t_token	*ft_lstnew(int quote_type, char *content, int len);
int		expand_env_var(t_token *tokens);
int		free_tokens(t_token **tokens);
int		parse_commands(t_token *tokens);
int		free_tokens_commands(t_token **tokens, t_command **commands);

#endif
