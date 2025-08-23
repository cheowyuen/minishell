/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:32:43 by cwong             #+#    #+#             */
/*   Updated: 2025/08/23 18:42:12 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_executor	t_executor;

typedef enum e_quote_type
{
	NONE,
	SINGLE,
	DOUBLE,
	DOUBLE_ENV
}							t_quote_type;

typedef struct s_token
{
	char					*token;
	t_quote_type			quote_type;
	bool					has_dollar_sign;
	struct s_token			*next;
}							t_token;

typedef struct s_var
{
	char					*value;
	bool					has_braces;
	bool					should_free;
	t_executor				*executor;
}							t_var;

typedef struct s_shell
{
	int						exit_code;
}							t_shell;

typedef struct s_quote
{
	char					*quoted_word;
	t_quote_type			quote_type;
	t_shell					*shell;
	t_executor				*executor;
}							t_quote;

typedef struct s_redir
{
	char    				*filename;
    bool    				append;
    struct s_redir			*next;
}   						t_redir;

typedef struct s_command
{
	char					**argv;
	char					*infile;
	int						append;
	int						has_heredoc;
	char					*heredoc_delim;
	int						is_builtin;
	int						is_last;
	int						pipe_out;
	t_redir					*outfiles;
	struct s_command		*next;
}							t_command;

int							parse_input(char *input, t_token **tokens,
								t_shell *shell, t_executor *executor);
int							free_tokens_input(t_token **tokens, char *input);
int							add_token(t_token **tokens, t_quote_type quote_type,
								char *content, int len);
int							is_special(char c);
int							is_quote(char c);
int							is_space(char c);
void						ft_lstclear(t_token **lst);
char						*ft_strndup(const char *src, int n);
int							free_tokens(t_token **tokens);
void						free_commands(t_command **commands);
int							parse_commands(t_token *tokens,
								t_command **commands);
int							free_tokens_commands(t_token **tokens,
								t_command **commands);
int							set_outfile(t_token **tokens, int append,
								t_redir **files);
int							set_infile(t_token **tokens, t_command *current);
int							is_builtin(char *cmd);
t_command					*create_command(void);
int							count_tokens(t_token *tokens);
int							get_word(char *input, int *i, t_quote *quote,
								char **word);
int							expand_env_var(char **word, t_shell *shell,
								t_executor *executor);
int							parse_quote(char *input, int *i, t_quote *quote);
int							handle_quoted_word(char **joined, t_quote *quote,
								char *input, int *i);
int							handle_unquoted_word(char **joined, t_quote *quote,
								char *input, int *i);

#endif
