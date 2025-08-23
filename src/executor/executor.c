/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:51:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/23 21:01:07 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "utils.h"
#include "signals.h"

int	exec_cmd(t_executor *executor, t_command *command)
{
	if (!executor || !command)
		return (FAILURE);
	if (!command->argv || !command->argv[0] || !command->argv[0][0])
	{
		if (!command->next && !command->pipe_out && !command->infile && 
			!command->outfiles && !command->has_heredoc)
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
	if (command->is_builtin)
		return (exec_builtin_with_redirection(executor, command));
	return (exec_single_external(executor, command));
}

int	setup_single_redirection(t_command *command)
{
	if (command->infile || command->has_heredoc)
	{
		if (setup_input_redirection(command) == FAILURE)
			return (FAILURE);
	}
	if (command->outfiles)
	{
		if (setup_output_redirection(command) == FAILURE)
			return (FAILURE);
	}
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
		if (setup_single_redirection(command) == FAILURE)
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
				write(STDOUT_FILENO, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
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
