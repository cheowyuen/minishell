/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:35:52 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/09 09:45:16 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "executor.h"
# include <stdio.h>
# include <string.h>
# include <unistd.h>

// Failure codes
# define FAILURE_WRITE -1

// Generic error handling
void	print_error_stderr(const char *msg);

// Generic memory management utilities
void	free_split(char **arr);
void	free_str_array(char **arr);
void	free_environment(t_executor *executor);
void	free_executor(t_executor *executor);
void	free_command(t_command *command);

#endif