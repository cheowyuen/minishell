/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:51:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/01 17:05:51 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"
#include "utils.h"

int	exec_cmd(t_executor *executor, t_command *command)
{
	if (!executor || !command)
		return (FAILURE);
	if (!command->argv)
	{
		if (!command->next && !command->pipe_out && !command->redirs
			&& !command->has_heredoc)
			return (SUCCESS);
	}
	executor->commands = command;
	if (command->next != NULL || command->pipe_out)
		return (exec_pipe(executor, command));
	else
		return (exec_single_command(executor, command));
}

int	exec_single_command(t_executor *executor, t_command *command)
{
	int	saved_stdin;
	int	saved_stdout;

	executor->command_count = 1;
	if (command->is_builtin)
		return (exec_builtin_with_redirection(executor, command));
	if (!command->argv || !command->argv[0])
	{
		if (command->redirs || command->has_heredoc)
		{
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
			if (saved_stdin < 0 || saved_stdout < 0)
				return (dup_failure(saved_stdin, saved_stdout));
			if (setup_redirection(command) == FAILURE)
			{
				restore_fds(saved_stdin, saved_stdout);
				return (FAILURE);
			}
			if (restore_fds(saved_stdin, saved_stdout) == FAILURE)
				return (FAILURE);
		}
		return (SUCCESS);
	}
	return (exec_single_external(executor, command));
}

int	exec_single_external(t_executor *executor, t_command *command)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		if (setup_redirection(command) == FAILURE)
		{
			free_inherited_state_for_child(executor, executor->commands);
			exit(EXIT_FAILURE);
		}
		exec_external_command(executor, command);
		exit(127);
	}
	else if (pid > 0)
	{
		return (handle_parent_process(pid));
	}
	perror("fork");
	return (FAILURE);
}

int	restore_fds(int saved_stdin, int saved_stdout)
{
	int	status;

	status = SUCCESS;
	if (saved_stdin >= 0)
	{
		if (dup2(saved_stdin, STDIN_FILENO) < 0)
		{
			perror("dup2 stdin restore");
			status = FAILURE;
		}
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		if (dup2(saved_stdout, STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout restore");
			status = FAILURE;
		}
		close(saved_stdout);
	}
	return (status);
}

int	dup_failure(int saved_stdin, int saved_stdout)
{
	perror("dup");
	if (saved_stdin >= 0)
		close(saved_stdin);
	if (saved_stdout >= 0)
		close(saved_stdout);
	return (FAILURE);
}
