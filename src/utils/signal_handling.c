/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:01:09 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/02 20:04:54 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

static int	set_last_status(t_executor *executor, pid_t pid, int status)
{
	int	i;

	i = 0;
	while (i < executor->command_count)
	{
		if (executor->pids[i] == pid)
		{
			if (i == executor->command_count - 1)
				return (status);
			break ;
		}
		i++;
	}
	return (-1);
}

int	wait_all_children(t_executor *executor, int *last_status)
{
	int		status;
	int		children_left;
	pid_t	pid;
	int		new_status;

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
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		new_status = set_last_status(executor, pid, status);
		if (new_status != -1)
			*last_status = new_status;
		children_left--;
	}
	return (SUCCESS);
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = SIGNAL_INT;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = SIGNAL_INT;
	rl_replace_line("", 0);
	rl_done = 1;
}

int	cleanup_heredoc_signals(bool is_interactive, int return_status)
{
	if (is_interactive)
	{
		setup_signals_prompt();
		rl_event_hook = NULL;
	}
	return (return_status);
}
