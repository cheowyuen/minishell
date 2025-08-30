/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:34 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 21:32:34 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"
#include "utils.h"

static int	setup_and_run_pipeline(t_executor *executor, t_command *command);
static int	process_pipeline_heredocs(t_command *command, t_executor *executor);

int	exec_pipe(t_executor *executor, t_command *command)
{
	if (!executor || !command)
		return (FAILURE);
	count_commands(command, executor);
	if (executor->command_count == 0)
		return (FAILURE);
	if (process_pipeline_heredocs(command, executor) == FAILURE)
		return (FAILURE);
	executor->current_command_index = 0;
	executor->pids = malloc(sizeof(pid_t) * executor->command_count);
	if (!executor->pids)
		return (FAILURE);
	return (setup_and_run_pipeline(executor, command));
}

static int	setup_and_run_pipeline(t_executor *executor, t_command *command)
{
	if (create_pipes(executor, executor->command_count) == FAILURE)
		return (clean_everything_error(executor));
	setup_signals_ignore();
	if (fork_processes(executor, command) == FAILURE)
		return (clean_everything_error(executor));
	close_and_free_pipes(executor);
	if (close_children_process(executor) == FAILURE)
		return (clean_everything_error(executor));
	setup_signals_prompt();
	if (executor->pids)
	{
		free(executor->pids);
		executor->pids = NULL;
	}
	return (executor->last_exit_code);
}

static int	process_pipeline_heredocs(t_command *command, t_executor *executor)
{
	t_command	*current;

	current = command;
	while (current)
	{
		if (current->has_heredoc)
		{
			if (setup_heredoc_for_pipeline(current, executor) == FAILURE)
				return (FAILURE);
		}
		current = current->next;
	}
	return (SUCCESS);
}

static int	allocate_pipes_array(t_executor *executor, int count)
{
	int	i;

	i = 0;
	executor->pipes = malloc(sizeof(int *) * (count - 1));
	if (!executor->pipes)
		return (FAILURE);
	while (i < count - 1)
	{
		executor->pipes[i] = NULL;
		i++;
	}
	return (SUCCESS);
}

int	create_pipes(t_executor *executor, int count)
{
	int	i;

	if (allocate_pipes_array(executor, count) == FAILURE)
		return (FAILURE);
	i = 0;
	while (i < count - 1)
	{
		executor->pipes[i] = malloc(sizeof(int) * 2);
		if (!executor->pipes[i])
		{
			executor->command_count = i + 1;
			break ;
		}
		executor->pipes[i][0] = FD_CLOSED;
		executor->pipes[i][1] = FD_CLOSED;
		if (pipe(executor->pipes[i]) == PIPE_ERROR)
		{
			free(executor->pipes[i]);
			executor->pipes[i] = NULL;
			executor->command_count = i + 1;
			break ;
		}
		i++;
	}
	return (SUCCESS);
}
