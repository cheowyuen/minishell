/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_setup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:00:00 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/03 19:29:32 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"
#include "utils.h"

static void	handle_child_process(t_executor *executor, t_command *command,
				int i);
static int	create_child_process(t_executor *executor, t_command *current_cmd,
				int i);

int	setup_children_pipes(t_executor *executor, int i)
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

static int	create_child_process(t_executor *executor, t_command *current_cmd,
		int i)
{
	pid_t	pid;

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
	}
	return (SUCCESS);
}

int	fork_processes(t_executor *executor, t_command *command)
{
	int			i;
	t_command	*current_cmd;

	i = 0;
	current_cmd = command;
	while (current_cmd && i < executor->command_count)
	{
		if (create_child_process(executor, current_cmd, i) == FAILURE)
			return (FAILURE);
		if (executor->pids[i] > 0)
		{
			i++;
			current_cmd = current_cmd->next;
		}
	}
	return (SUCCESS);
}

static void	handle_child_process(t_executor *executor, t_command *command,
		int i)
{
	t_command *original_commands;
	
	if (setup_children_pipes(executor, i) == FAILURE)
	{
		close_all_pipes_children(executor);
		free_inherited_state_for_child(executor, executor->commands);
		exit(EXIT_FAILURE);
	}
	setup_redirection_child_process(executor, command);
	close_all_pipes_children(executor);
	if (command->is_builtin)
	{
		original_commands = executor->commands;
		executor->commands = command;
		exec_builtin(executor);
		free_inherited_state_for_child(executor, original_commands);
		exit(executor->last_exit_code);
	}
	else if (!command->argv || !command->argv[0] || !command->argv[0][0])
	{
		free_inherited_state_for_child(executor, executor->commands);
		exit(SUCCESS);
	}
	else
		exec_external_command(executor, command);
}

void	setup_redirection_child_process(t_executor *executor,
		t_command *command)
{
	if (command->has_heredoc && command->heredoc_fd >= 0)
	{
		if (dup2(command->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			close_all_pipes_children(executor);
			exit(EXIT_FAILURE);
		}
		close(command->heredoc_fd);
		command->heredoc_fd = -1;
	}
	if (command->redirs)
	{
		if (setup_redirection(command) == FAILURE)
		{
			close_all_pipes_children(executor);
			exit(EXIT_FAILURE);
		}
	}
}
