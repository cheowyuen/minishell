/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:49:22 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/28 19:58:15 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "executor.h"
# include "minishell.h"
# include "utils.h"
# include <errno.h>

# define EXPORTED 1
# define NOT_EXPORTED 0
# define EQUAL 0

typedef struct s_builtin_entry
{
	char	*name;
	void	(*func)(t_executor *executor);
}			t_builtin_entry;

// Builtin command functions
void		builtin_echo(t_executor *executor);
void		builtin_pwd(t_executor *executor);
void		builtin_cd(t_executor *executor);
void		builtin_export(t_executor *executor);
void		builtin_unset(t_executor *executor);
void		builtin_env(t_executor *executor);
void		builtin_exit(t_executor *executor);

// Export utility functions
int			add_env_variable(t_executor *executor, const char *var);
int			add_var_to_list(t_executor *executor, const char *key,
				const char *value);
int			is_valid_identifier(const char *str);
int			is_env_key_set(t_executor *executor, const char *key);
char		*get_env_var(t_executor *executor, const char *key);

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
char		*resolve_logical_path(char *current_logical_pwd, char *target);
char		*prepare_path_for_resolution(char *current_logical_pwd,
				char *target);
char		*resolve_path_components(char **components, int num_components);
char		*join_path_components(char **resolved_parts, int count);

#endif