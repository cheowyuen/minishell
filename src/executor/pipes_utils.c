/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:04:18 by lsurco-t          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/08/19 09:09:07 by cwong            ###   ########.fr       */
=======
/*   Updated: 2025/08/18 20:00:38 by lsurco-t         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "utils.h"
#include "signals.h"

void	close_and_free_pipes(t_executor *executor)
{
	int	i;

	i = 0;
	while (i < executor->command_count - 1)
	{
		if (executor->pipes[i])
		{
			if (executor->pipes[i][0] >= 0)
			{
				close(executor->pipes[i][0]);
				executor->pipes[i][0] = FD_CLOSED;
			}
			if (executor->pipes[i][1] >= 0)
			{
				close(executor->pipes[i][1]);
				executor->pipes[i][1] = FD_CLOSED;
			}
			free(executor->pipes[i]);
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
	int		last_status;

	last_status = 0;
	if (wait_all_children(executor, &last_status) < 0)
		return (FAILURE);
	if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	if (WIFEXITED(last_status))
		executor->last_exit_code = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
		executor->last_exit_code = EXIT_SIGNAL_BASE + WTERMSIG(last_status);
	return (SUCCESS);
}


void	setup_redirection_child_process(t_executor *executor,
		t_command *command)
{
	if (command->infile || command->has_heredoc)
	{
		if (setup_input_redirection(command) == FAILURE)
		{
			close_all_pipes_children(executor);
			exit(EXIT_FAILURE);
		}
	}
	if (command->outfile)
	{
		if (setup_output_redirection(command) == FAILURE)
		{
			close_all_pipes_children(executor);
			exit(EXIT_FAILURE);
		}
	}
}
