/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:32:43 by cwong             #+#    #+#             */
/*   Updated: 2025/09/06 12:57:08 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define HEREDOC_LIMIT_EXCEEDED -2
# define HEREDOC_INTERRUPTED -3

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND
}							t_redir_type;

typedef struct s_executor	t_executor;

typedef struct s_token
{
	char					*token;
	bool					is_quoted;
	bool					has_dollar_sign;
	char					*raw_value;
	int						words_count;
	struct s_token			*next;
}							t_token;

typedef struct s_token_data
{
	int						len;
	char					*raw_value;
	int						words_count;
}							t_token_data;

typedef struct s_word
{
	int						*i;
	bool					is_quoted;
	bool					has_unquoted_vars;
	char					*temp;
	int						starting_pos;
}							t_word;

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
	char					*raw_value;
	t_executor				*executor;
	bool					is_heredoc;
}							t_quote;

typedef struct s_heredoc_list
{
	char					*delimiter;
	bool					is_quoted;
	struct s_heredoc_list	*next;
}							t_heredoc_list;

typedef struct s_redir
{
	t_redir_type			type;
	char					*filename;
	struct s_redir			*next;
}							t_redir;

typedef struct s_command
{
	char					**argv;
	int						append;
	int						has_heredoc;
	char					*heredoc_delim;
	char					*heredoc_content;
	int						heredoc_fd;
	bool					is_quoted;
	int						is_builtin;
	int						is_last;
	int						pipe_out;
	t_heredoc_list			*heredoc_list;
	t_redir					*redirs;
	struct s_command		*next;
}							t_command;

// Expansion
int							expand_env_var(char **word, t_executor *executor);
int							expand_tilde(char **word, t_executor *executor);
int							expand_word_if_needed(char **word, bool has_dollar,
								t_executor *executor);
int							handle_exit_code(const char *word, int i,
								t_var *var, t_executor *executor);

// Parse commands			
int							parse_commands(t_token *tokens,
								t_command **commands, t_executor *executor);
int							handle_special(t_token **tokens,
								t_command **current, int *heredoc_count,
								t_executor *executor);
int							fill_argv_from_tokens(t_token **tokens,
								t_command *current, t_executor *executor,
								int *heredoc_count);
int							handle_heredoc(t_token **tokens,
								t_command *current, int *heredoc_count,
								t_executor *executor);

// Parse commands utilities
int							set_redir(t_token **tokens, t_redir **files);
t_command					*create_command(void);
int							is_builtin(char *cmd);
int							count_tokens(t_token *tokens);

// Cleanup
int							free_tokens_input(t_token **tokens, char *input,
								int flag);
int							free_tokens_commands(t_token **tokens,
								t_command **commands);
void						free_commands(t_command **commands);

// Parser list utilities
int							add_token(t_token **tokens, bool is_quoted,
								char *content, t_token_data *data);
t_token						*ft_lstnew(bool is_quoted, char *content,
								t_token_data *data);
void						ft_lstadd_back(t_token **lst, t_token *new);
void						ft_lstclear(t_token **lst);
int							free_tokens(t_token **tokens);

// Parser utilities
int							is_special(char c);
int							is_quote(char c);
int							is_space(char c);
char						*ft_strndup(const char *src, int n);

// Parser word utilies
int							handle_quoted_word(char **joined, t_quote *quote,
								char *input, int *i);
int							handle_unquoted_word(char **joined, t_quote *quote,
								char *input, int *i);
int							get_word(char *input, int *i, t_quote *quote,
								char **word);

// Parser
int							parse_input(char *input, t_token **tokens,
								t_executor *executor);
int							parse_quote(char *input, int *i, t_quote *quote);
int							parse_word(char *input, int *i, t_token **tokens,
								t_quote *quote);
int							add_tokens_with_splitting(t_token **tokens,
								bool is_quoted, char *content, char *raw_value);
int							handle_quoted(t_word *w, char **joined,
								t_quote *quote, char *input);
int							parse_and_execute(char *input, t_token **tokens,
								t_executor *executor);

// Syntax validation
int							validate_syntax(t_token *tokens,
								t_executor *executor);

// Heredoc functions
t_heredoc_list				*add_heredoc_to_list(t_heredoc_list **list,
								char *delimiter, bool is_quoted);
int							create_heredoc_pipe(t_command *command,
								char *content);
void						free_heredoc_list(t_heredoc_list *list);
int							process_all_heredocs(t_command *command,
								t_executor *executor);
int							read_heredoc_input(char *delimiter, bool is_quoted,
								t_executor *executor, char **content);
int							cleanup_eof_warning(char *delimiter,
								bool is_interactive);
char						*process_heredoc_delimiter(const char *delimiter);

#endif
