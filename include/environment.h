/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 09:33:28 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/06 12:56:56 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "executor.h"

// PWD and OLDPWD management
int		set_oldpwd(t_executor *executor);
int		update_oldpwd(t_executor *executor);
int		update_pwd(t_executor *executor);

// Environment utility functions
int		is_env_key_set(t_executor *executor, const char *key);

// PATH-related functions
char	*find_command_path(t_executor *executor, char *command);

#endif
