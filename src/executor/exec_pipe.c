/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:34 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 16:24:35 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"
#include "utils.h"

static int	create_pipes(t_executor *executor, int count);
static int	fork_processes(t_executor *executor, t_command *command);
static int	setup_children_pipes(t_executor *executor, int i);
static void	handle_child_process(t_executor *executor, t_command *command,
				int i);
static int	process_pipeline_heredocs(t_command *command, t_executor *executor);

int	exec_pipe(t_executor *executor, t_command *command)
{
	if (!executor || !command)
		return (FAILURE);
	count_commands(command, executor);
	if (executor->command_count == 0)
		return (FAILURE);
	// Process all heredocs BEFORE starting the pipeline
	if (process_pipeline_heredocs(command, executor) == FAILURE)
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
	// Initialize all pointers to NULL for safe cleanup
	while (i < count - 1)
	{
		executor->pipes[i] = NULL;
		i++;
	}
	i = 0;
	while (i < count - 1)
	{
		executor->pipes[i] = malloc(sizeof(int) * 2);
		if (!executor->pipes[i])
		{
			// If malloc fails, we can't create more pipes but can continue with what we have
			executor->command_count = i + 1;  // Adjust command count to match available pipes
			break ;
		}
		executor->pipes[i][0] = FD_CLOSED;
		executor->pipes[i][1] = FD_CLOSED;
		if (pipe(executor->pipes[i]) == PIPE_ERROR)
		{
			// If pipe() fails, silently continue with reduced pipeline like bash
			free(executor->pipes[i]);
			executor->pipes[i] = NULL;
			executor->command_count = i + 1;  // Adjust command count to match available pipes
			break ;
		}
		i++;
	}
	// Even if we couldn't create all pipes, we can still execute some commands
	return (SUCCESS);
}

static int	fork_processes(t_executor *executor, t_command *command)
{
	pid_t		pid;
	int			i;
	t_command	*current_cmd;

	i = 0;
	current_cmd = command;
	while (current_cmd && i < executor->command_count)
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
			handle_child_process(executor, current_cmd, i);
		}
		else
		{
			executor->pids[i] = pid;
			i++;
			current_cmd = current_cmd->next;
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
	else if (!command->argv || !command->argv[0] || !command->argv[0][0])
	{
		free_inherited_state_for_child(executor, command);
		exit(SUCCESS);
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
