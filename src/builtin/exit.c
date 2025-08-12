/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:15 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/11 19:26:20 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/builtin.h"

void	builtin_exit(t_executor *executor, t_command *command)
{
	if (command && command->argv)
		free_split(command->argv);
	if (executor->env_list)
		free_environment(executor);
	if (executor)
		free_executor(executor);
	if (executor->last_exit_code != SUCCESS)
	{
		ft_dprintf(STDERR_FILENO, "exit: last command failed with code %d\n",
			executor->last_exit_code);
	}
	else
	{
		ft_dprintf(STDOUT_FILENO, "Exiting minishell with code %d\n",
			executor->last_exit_code);
	}
	exit(executor->last_exit_code);
}
