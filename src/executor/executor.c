/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:51:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 21:01:12 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "utils.h"
#include "signals.h"

int	exec_cmd(t_executor *executor, t_command *command)
{
	if (!executor || !command)
		return (FAILURE);
	if (!command->argv)
	{
		if (!command->next && !command->pipe_out && !command->redirs && !command->has_heredoc)
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
	int saved_stdin;
	int saved_stdout;

	executor->command_count = 1;
	if (command->is_builtin)
		return (exec_builtin_with_redirection(executor, command));
	if (!command->argv || !command->argv[0])
	{
		if (command->redirs || command->has_heredoc)
		{
			saved_stdin = -1;
			saved_stdout = -1;
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
			if (setup_redirection(command, executor) == FAILURE)
			{
				restore_fds(saved_stdin, saved_stdout);
				return (FAILURE);
			}
			restore_fds(saved_stdin, saved_stdout);
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
		if (setup_redirection(command, executor) == FAILURE)
		{
			free_inherited_state_for_child(executor, command);
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

void	restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
