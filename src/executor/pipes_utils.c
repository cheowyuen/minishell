/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:04:18 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/05 10:23:11 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"

static void	close_and_free_single_pipe(int *pipe_fds)
{
	if (pipe_fds[0] >= 0)
	{
		close(pipe_fds[0]);
		pipe_fds[0] = FD_CLOSED;
	}
	if (pipe_fds[1] >= 0)
	{
		close(pipe_fds[1]);
		pipe_fds[1] = FD_CLOSED;
	}
	free(pipe_fds);
}

void	close_and_free_pipes(t_executor *executor)
{
	int	i;

	if (!executor || !executor->pipes)
		return ;
	i = 0;
	while (i < executor->command_count - 1)
	{
		if (executor->pipes[i])
		{
			close_and_free_single_pipe(executor->pipes[i]);
			executor->pipes[i] = NULL;
		}
		i++;
	}
	free(executor->pipes);
	executor->pipes = NULL;
}

void	close_all_pipes_children(t_executor *executor)
{
	int	i;

	if (!executor || !executor->pipes)
		return ;
	i = 0;
	while (i < executor->command_count - 1)
	{
		if (executor->pipes[i])
		{
			if (executor->pipes[i][0] >= 0)
				close(executor->pipes[i][0]);
			if (executor->pipes[i][1] >= 0)
				close(executor->pipes[i][1]);
		}
		i++;
	}
}

int	clean_everything_error(t_executor *executor)
{
	int	i;
	int	status;

	if (!executor)
		return (FAILURE);
	close_and_free_pipes(executor);
	if (executor->pids)
	{
		i = 0;
		while (i < executor->command_count)
		{
			if (executor->pids[i] > 0)
				waitpid(executor->pids[i], &status, 0);
			i++;
		}
		free(executor->pids);
		executor->pids = NULL;
	}
	return (FAILURE);
}

int	close_children_process(t_executor *executor)
{
	int	last_status;

	last_status = 0;
	if (wait_all_children(executor, &last_status) < 0)
		return (FAILURE);
	if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	if (WIFSIGNALED(last_status))
		executor->last_exit_code = EXIT_SIGNAL_BASE + WTERMSIG(last_status);
	else if (WIFEXITED(last_status))
		executor->last_exit_code = WEXITSTATUS(last_status);
	return (SUCCESS);
}
