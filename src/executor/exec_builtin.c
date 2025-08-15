/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:00:21 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:35:40 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

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
	ft_dprintf(STDERR_FILENO, "%s: command not found\n",
		executor->commands->argv[0]);
	executor->last_exit_code = FAILURE;
	return (FAILURE);
}

int	exec_builtin_with_redirection(t_executor *executor, t_command *command)
{
	int	result;
	int	saved_stdin;
	int	saved_stdout;

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
	if (setup_single_redirection(command) == FAILURE)
	{
		restore_fds(saved_stdin, saved_stdout);
		return (FAILURE);
	}
	result = exec_builtin(executor);
	restore_fds(saved_stdin, saved_stdout);
	return (result);
}
