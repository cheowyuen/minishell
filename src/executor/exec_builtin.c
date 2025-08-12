/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:00:21 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/11 19:39:37 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/builtin.h"

int	exec_builtin(t_executor *executor, t_command *command)
{
	const t_builtin_entry	list_builtins[] = {{"echo", builtin_echo}, {"pwd",
		builtin_pwd}, {"cd", builtin_cd}, {"export", builtin_export},
	{"unset", builtin_unset}, {"env", builtin_env}, {"exit",
		builtin_exit}, {NULL, NULL}};
	int						i;

	i = 0;
	if (!command->is_builtin || !command->argv[0] || !executor)
		return (FAILURE);
	while (list_builtins[i].name)
	{
		if (ft_strcmp(command->argv[0], list_builtins[i].name) == EQUAL)
		{
			list_builtins[i].func(executor, command);
			return (SUCCESS);
		}
		i++;
	}
	ft_dprintf(STDERR_FILENO, "%s: command not found\n",
		command->argv[0]);
	executor->last_exit_code = FAILURE;
	return (FAILURE);
}
