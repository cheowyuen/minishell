/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:51:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 15:38:43 by lsurco-t         ###   ########.fr       */
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
	// Set command count to 1 for single commands
	executor->command_count = 1;
	if (command->is_builtin)
		return (exec_builtin_with_redirection(executor, command));
	// Handle empty commands (like when no tokens exist or only redirections)
	if (!command->argv || !command->argv[0])
	{
		// If there are redirections or heredocs, handle them
		if (command->redirs || command->has_heredoc)
		{
			int saved_stdin = -1;
			int saved_stdout = -1;
			
			// Save original file descriptors
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
			
			// Setup redirections
			if (setup_single_redirection(command, executor) == FAILURE)
			{
				restore_fds(saved_stdin, saved_stdout);
				return (FAILURE);
			}
			// Restore file descriptors
			restore_fds(saved_stdin, saved_stdout);
		}
		return (SUCCESS);
	}
	return (exec_single_external(executor, command));
}

int	setup_single_redirection(t_command *command, t_executor *executor)
{
	if (command->redirs)
	{
		if (setup_redirection(command, executor) == FAILURE)
			return (FAILURE);
	}
	if (command->has_heredoc)
	{
		if (setup_input_redirection(command, executor) == FAILURE)
			return (FAILURE);
	}
	// if (command->outfiles)
	// {
	// 	if (setup_output_redirection(command) == FAILURE)
	// 		return (FAILURE);
	// }
	return (SUCCESS);
}

int	exec_single_external(t_executor *executor, t_command *command)
{
	pid_t	pid;
	int		status;
	
	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		if (setup_single_redirection(command, executor) == FAILURE)
		{
			free_inherited_state_for_child(executor, command);
			exit(EXIT_FAILURE);
		}
		exec_external_command(executor, command);
		exit(127);
	}
	else if (pid > 0)
	{
		setup_signals_ignore();  // Parent ignores signals while waiting
		if (waitpid(pid, &status, 0) < 0)
		{
			perror("waitpid");
			setup_signals_prompt();  // Restore parent signals
			return (FAILURE);
		}
		setup_signals_prompt();  // Restore parent signals
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				write(STDERR_FILENO, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			return (EXIT_SIGNAL_BASE + WTERMSIG(status));
		}
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
