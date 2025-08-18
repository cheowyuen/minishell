/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:01:09 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/18 13:01:42 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

int wait_for_child(pid_t child_pid)
{
    int		status;
    int		last_status;
	pid_t	pid;

	last_status = 0;
    while (1)
    {
        pid = waitpid(child_pid, &status, 0);
        if (pid == WAITPID_ERROR)
        {
			if (errno == EINTR)
            	continue ;
            if (errno == ECHILD)
                break ;
			perror("waitpid");
			return (FAILURE);
        }
        last_status = status;
        break;
    }
    if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGINT)
        write(STDOUT_FILENO, "\n", 1);
    return (SUCCESS);
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
