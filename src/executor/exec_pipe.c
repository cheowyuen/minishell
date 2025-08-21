/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:34 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/21 19:03:37 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "utils.h"
#include "signals.h"

static int	create_pipes(t_executor *executor, int count);
static int	fork_processes(t_executor *executor, t_command *command);
static int	setup_children_pipes(t_executor *executor, int i);
static void	handle_child_process(t_executor *executor, t_command *command,
				int i);

int	exec_pipe(t_executor *executor, t_command *command)
{
	if (!executor || !command)
		return (FAILURE);
	count_commands(command, executor);
	if (executor->command_count == 0)
		return (FAILURE);
	executor->current_command_index = 0;
	executor->pids = malloc(sizeof(pid_t) * executor->command_count);
	if (!executor->pids)
		return (FAILURE);
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

static int	create_pipes(t_executor *executor, int count)
{
	int	i;

	i = 0;
	executor->pipes = malloc(sizeof(int *) * (count - 1));
	if (!executor->pipes)
		return (FAILURE);
	while (i < count - 1)
	{
		executor->pipes[i] = malloc(sizeof(int) * 2);
		if (!executor->pipes[i])
		{
			close_and_free_pipes(executor);
			return (FAILURE);
		}
		executor->pipes[i][0] = FD_CLOSED;
		executor->pipes[i][1] = FD_CLOSED;
		if (pipe(executor->pipes[i]) == PIPE_ERROR)
		{
			perror("pipe");
			close_and_free_pipes(executor);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

static int	fork_processes(t_executor *executor, t_command *command)
{
	pid_t	pid;
	int		i;
	
	i = 0;
	while (command)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return (FAILURE);
		}
		if (pid == 0)
		{
			setup_signals_child();
			handle_child_process(executor, command, i);
		}
		else
		{
			executor->pids[i] = pid;
			i++;
			command = command->next;
		}
	}
	return (SUCCESS);
}

static void	handle_child_process(t_executor *executor, t_command *command,
		int i)
{
	if (setup_children_pipes(executor, i) == FAILURE)
	{
		close_all_pipes_children(executor);
		free_inherited_state_for_child(executor, command);
		exit(EXIT_FAILURE);
	}
	setup_redirection_child_process(executor, command);
	close_all_pipes_children(executor);
	if (command->is_builtin)
	{
		executor->commands = command;
		exec_builtin(executor);
		free_inherited_state_for_child(executor, command);
		exit(executor->last_exit_code);
	}
	else
		exec_external_command(executor, command);
}

static int	setup_children_pipes(t_executor *executor, int i)
{
	if (i > 0)
	{
		if (dup2(executor->pipes[i - 1][0], STDIN_FILENO) == DUP2_ERROR)
		{
			perror("dup2");
			return (FAILURE);
		}
		close(executor->pipes[i - 1][0]);
		executor->pipes[i - 1][0] = FD_CLOSED;
	}
	if (i < executor->command_count - 1)
	{
		if (dup2(executor->pipes[i][1], STDOUT_FILENO) == DUP2_ERROR)
		{
			perror("dup2");
			return (FAILURE);
		}
		close(executor->pipes[i][1]);
		executor->pipes[i][1] = FD_CLOSED;
	}
	return (SUCCESS);
}
