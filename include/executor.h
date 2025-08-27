/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuen <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:20:17 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/27 19:34:47 by cyuen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "parser.h"

// System call error codes
# define PIPE_ERROR -1
# define DUP2_ERROR -1
# define FORK_ERROR -1
# define WAITPID_ERROR -1

// File descriptor states
# define FD_CLOSED -1
# define FD_INVALID -1

// Exit codes
# define EXIT_CMD_NOT_EXECUTABLE 126
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_SIGNAL_BASE 128

# define INVALID_ID "not a valid identifier"

// Max heredoc limit per command line
# define HEREDOC_MAX 16

// Environment variable structure to save key-value pairs
typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}					t_env;

// Executor structure to manage command execution
typedef struct s_executor
{
	t_command		*commands;
	t_env			*env_list;
	pid_t			*pids;
	int				command_count;
	int				current_command_index;
	int				last_exit_code;
	int				**pipes;
	char			*logical_pwd;
}					t_executor;

// Functions command execution
int					exec_cmd(t_executor *executor, t_command *command);
int					exec_builtin(t_executor *executor);
int					exec_builtin_with_redirection(t_executor *executor,
						t_command *command);
int					exec_pipe(t_executor *executor, t_command *command);
void				exec_external_command(t_executor *executor,
						t_command *command);

// Execute single command
int					exec_single_command(t_executor *executor,
						t_command *command);
void				restore_fds(int saved_stdin, int saved_stdout);
int					setup_single_redirection(t_command *command, t_executor *executor);
int					exec_single_external(t_executor *executor,
						t_command *command);

// Heredoc management
int					setup_heredoc(t_command *command, t_executor *executor);
int					setup_input_redirection(t_command *command, t_executor *executor);
// int					setup_output_redirection(t_command *command);
int					setup_redirection(t_command *command, t_executor *executor);

// Execution utilities
char				**build_env_array(t_executor *executor);
void				count_commands(t_command *command, t_executor *executor);
void				add_envp_to_executor(t_executor *executor, char **envp);

// Pipe utils
int					clean_everything_error(t_executor *executor);
void				close_and_free_pipes(t_executor *executor);
void				setup_redirection_child_process(t_executor *executor,
						t_command *command);
void				close_all_pipes_children(t_executor *executor);
int					close_children_process(t_executor *executor);

// Executor memory management
void				free_environment(t_executor *executor);
void				free_executor(t_executor *executor);

#endif
