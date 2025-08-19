/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:01:09 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/19 11:43:25 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

static int	set_last_status(t_executor *executor, int pid, int status)
{
	int	i;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < executor->command_count)
	{
		if (executor->pids[i] == pid)
		{
			if (i == executor->command_count - 1)
				last_status = status;
			break ;
		}
		i++;
	}
	return (last_status);
}

int	wait_all_children(t_executor *executor, int *last_status)
{
	int		status;
	int		children_left;
	pid_t	pid;

	children_left = executor->command_count;
	while (children_left > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == WAITPID_ERROR)
		{
			if (errno == ECHILD)
				break ;
			perror("waitpid");
			return (FAILURE);
		}
		*last_status = set_last_status(executor, pid, status);
		children_left--;
	}
	return (SUCCESS);
}

// int	wait_for_child(pid_t pid)
// {
// 	int		status;

// 	if (waitpid(pid, &status, 0) == WAITPID_ERROR)
// 	{
// 		perror("waitpid");
// 		return (FAILURE);
// 	}
// 	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
// 		write(STDOUT_FILENO, "\n", 1);
// 	return (SUCCESS);
// }

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = SIGNAL_INT;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
