/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:35:52 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/23 20:46:31 by cheowyuenwo      ###   ########.fr       */
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
int		is_only_space_or_tab(char *str);

// Generic memory management utilities
void	free_split(char **arr);
void	free_str_array(char **arr);
void	free_environment(t_executor *executor);
void	free_executor(t_executor *executor);
int		free_input_failure(char *input);

// Free inherited parent state in a forked child before exit
void	free_inherited_state_for_child(t_executor *executor,
			t_command *commands);

// Sort linked list of environment variables alphabetically by key
void	sort_env_list_alpha(t_env **env_list);

char	*get_next_line(int fd);

#endif