/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:15 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:34:31 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	builtin_exit(t_executor *executor)
{
	if (!executor || !executor->commands || !executor->commands->argv)
	{
		ft_dprintf(STDERR_FILENO, "exit: no command to exit from\n");
		return ;
	}
	if (executor->commands && executor->commands->argv)
		free_split(executor->commands->argv);
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
		ft_dprintf(STDOUT_FILENO, "exit\n", executor->last_exit_code);
	}
	exit(executor->last_exit_code);
}
