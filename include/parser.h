/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:32:43 by cwong             #+#    #+#             */
/*   Updated: 2025/08/30 19:51:58 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define HEREDOC_LIMIT_EXCEEDED -2

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}							t_redir_type;

typedef struct s_executor	t_executor;

typedef struct s_token
{
	char					*token;
	bool					is_quoted;
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

typedef struct s_quote
{
	char					*quoted_word;
	bool					is_heredoc;
	t_executor				*executor;
}							t_quote;

typedef struct s_redir
{
	t_redir_type			type;
	char					*filename;
	struct s_redir			*next;
}							t_redir;

typedef struct s_infile
{
	char					*filename;
	struct s_infile			*next;
}							t_infile;

typedef struct s_outfile
{
	char					*filename;
	bool					append;
	struct s_outfile		*next;
}							t_outfile;

typedef struct s_command
{
	char					**argv;
	t_infile				*infiles;
	int						append;
	int						has_heredoc;
	char					*heredoc_delim;
	int						heredoc_fd;
	bool					is_quoted;
	int						is_builtin;
	int						is_last;
	int						pipe_out;
	t_outfile				*outfiles;
	t_redir					*redirs;
	struct s_command		*next;
}							t_command;

int							parse_input(char *input, t_token **tokens,
								t_executor *executor);
int							free_tokens_input(t_token **tokens, char *input,
								int flag);
int							add_token(t_token **tokens, bool is_quoted,
								char *content, int len);
int							add_tokens_with_splitting(t_token **tokens,
								bool is_quoted, char *content);
int							is_special(char c);
int							is_quote(char c);
int							is_space(char c);
void						ft_lstclear(t_token **lst);
char						*ft_strndup(const char *src, int n);
int							free_tokens(t_token **tokens);
void						free_commands(t_command **commands);
int							parse_commands(t_token *tokens,
								t_command **commands, t_executor *executor);
int							free_tokens_commands(t_token **tokens,
								t_command **commands);
int							set_outfile(t_token **tokens, int append,
								t_outfile **files);
int							set_infile(t_token **tokens, t_infile **files);
int							is_builtin(char *cmd);
t_command					*create_command(void);
int							count_tokens(t_token *tokens);
int							expand_env_var(char **word, t_executor *executor);
int							expand_tilde(char **word, t_executor *executor);
char						*process_heredoc_delimiter(const char *raw_delimiter,
								bool is_quoted, t_executor *executor);
int							parse_quote(char *input, int *i, t_quote *quote);
int							handle_quoted_word(char **joined, t_quote *quote,
								char *input, int *i);
int							handle_unquoted_word(char **joined, t_quote *quote,
								char *input, int *i);
int							get_word(char *input, int *i, t_quote *quote,
								char **word);
int							validate_syntax(t_token *tokens,
								t_executor *executor);
int							get_word_heredoc(char *input, int *i,
								t_quote *quote, char **word);
int							set_redir(t_token **tokens, t_redir **files);

#endif
