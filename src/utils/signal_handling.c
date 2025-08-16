/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:01:09 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/16 16:44:50 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

int	wait_for_child(pid_t pid)
{
	int		child_status;
	int		ret;

	while (1)
	{
		ret = waitpid(pid, &child_status, 0);
		if (ret > 0)
		{
			printf("[Parent] waitpid returned, child %d exited\n", pid);
			return SUCCESS;
		}
		if (ret < 0)
		{
			if (errno == EINTR && g_signal_status == SIGNAL_INT)
			{
				printf("[Parent] waitpid interrupted by SIGINT, sending SIGINT to child %d\n", pid);
				kill(pid, SIGINT);
				g_signal_status = 0;
				continue;
			}
			else if (errno != ECHILD)
			{
				perror("waitpid");
				return FAILURE;
			}
			break;
		}
	}
	return SUCCESS;
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = SIGNAL_INT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
