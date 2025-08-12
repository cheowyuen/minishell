/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:51:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/11 20:54:56 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

static int	exec_single_command(t_executor *executor, t_command *command);
static int	setup_single_redirection(t_command *command);
static int	exec_single_external(t_executor *executor, t_command *command);
static void	restore_fds(int saved_stdin, int saved_stdout);

int	exec_cmd(t_executor *executor, t_command *command)
{
	if (!executor || !command)
		return (FAILURE);
	executor->commands = command;
	if (command->next != NULL || command->pipe_out)
		return (exec_pipe(executor, command));
	else
		return (exec_single_command(executor, command));
	return (SUCCESS);
}

static int	exec_single_command(t_executor *executor, t_command *command)
{
	int	result;
	int	saved_stdin;
	int	saved_stdout;

	if (!executor || !command)
		return (FAILURE);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (setup_single_redirection(command) == FAILURE)
	{
		restore_fds(saved_stdin, saved_stdout);
		return (FAILURE);
	}
	if (command->is_builtin)
		result = exec_builtin(executor, command);
	else
		result = exec_single_external(executor, command);
	restore_fds(saved_stdin, saved_stdout);
	return (result);
}

static int	setup_single_redirection(t_command *command)
{
	if (command->infile || command->has_heredoc)
	{
		if (setup_input_redirection(command) == FAILURE)
			return (FAILURE);
	}
	if (command->outfile)
	{
		if (setup_output_redirection(command) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

static int	exec_single_external(t_executor *executor, t_command *command)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		exec_external_command(executor, command);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (EXIT_SIGNAL_BASE + WTERMSIG(status));
	}
	else
	{
		perror("fork");
		return (FAILURE);
	}
	return (SUCCESS);
}

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
		dup2(saved_stdin, STDIN_FILENO);
	if (saved_stdout >= 0)
		dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
