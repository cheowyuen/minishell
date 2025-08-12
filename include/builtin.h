/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:49:22 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/11 14:53:36 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "executor.h"
# include "minishell.h"
# include "utils.h"

# define EXPORTED 1
# define NOT_EXPORTED 0
# define EQUAL 0

typedef struct s_builtin_entry
{
	char	*name;
	void	(*func)(t_executor *executor, t_command *command);
}			t_builtin_entry;

// Builtin command functions
void		builtin_echo(t_executor *executor, t_command *command);
void		builtin_pwd(t_executor *executor, t_command *command);
void		builtin_cd(t_executor *executor, t_command *command);
void		builtin_export(t_executor *executor, t_command *command);
void		builtin_unset(t_executor *executor, t_command *command);
void		builtin_env(t_executor *executor, t_command *command);
void		builtin_exit(t_executor *executor, t_command *command);

// Export utility functions
int			add_env_variable(t_executor *executor, const char *var);
int			is_valid_identifier(const char *str);
int			add_env_to_list(t_executor *executor, const char *key,
				const char *value);
int			is_env_key_set(t_executor *executor, const char *key);
char		*get_env_value(t_executor *executor, const char *key);

// Cd functions
char		*handle_cd_dash(t_executor *executor);
char		*handle_cd_home(t_executor *executor);

// Cd utility functions
void		cleanup_and_set_failure(t_executor *executor, int need_free,
				char *target_copy);
int			perform_directory_change(t_executor *executor, char *target_copy,
				int need_free);
char		*get_target_directory(t_executor *executor, t_command *command,
				int *need_free);

#endif