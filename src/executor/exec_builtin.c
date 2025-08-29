/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:00:21 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/29 10:45:12 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	exec_builtin_and_restore(t_executor *executor, int saved_stdin,
				int saved_stdout, int is_exit_command);

int	exec_builtin(t_executor *executor)
{
	const t_builtin_entry	list_builtins[] = {{"echo", builtin_echo}, {"pwd",
		builtin_pwd}, {"cd", builtin_cd}, {"export", builtin_export},
	{"unset", builtin_unset}, {"env", builtin_env}, {"exit",
		builtin_exit}, {NULL, NULL}};
	int						i;

	i = 0;
	if (!executor || !executor->commands)
		return (FAILURE);
	if (!executor->commands->is_builtin || !executor->commands->argv[0])
		return (FAILURE);
	while (list_builtins[i].name)
	{
		if (ft_strcmp(executor->commands->argv[0],
				list_builtins[i].name) == EQUAL)
		{
			list_builtins[i].func(executor);
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}

int	exec_builtin_with_redirection(t_executor *executor, t_command *command)
{
	int	saved_stdin;
	int	saved_stdout;
	int	is_exit_command;

	is_exit_command = (ft_strcmp(command->argv[0], "exit") == 0);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		perror("dup");
		if (saved_stdin >= 0)
			close(saved_stdin);
		if (saved_stdout >= 0)
			close(saved_stdout);
		return (FAILURE);
	}
	if (setup_single_redirection(command, executor) == FAILURE)
	{
		restore_fds(saved_stdin, saved_stdout);
		return (FAILURE);
	}
	return (exec_builtin_and_restore(executor, saved_stdin, saved_stdout,
			is_exit_command));
}

static int	exec_builtin_and_restore(t_executor *executor, int saved_stdin,
		int saved_stdout, int is_exit_command)
{
	if (is_exit_command)
	{
		restore_fds(saved_stdin, saved_stdout);
		exec_builtin(executor);
		return (executor->last_exit_code);
	}
	exec_builtin(executor);
	restore_fds(saved_stdin, saved_stdout);
	return (executor->last_exit_code);
}
