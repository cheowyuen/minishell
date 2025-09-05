/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:15 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/05 10:44:08 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	validate_exit_args(char **argv, int *exit_code);

void	builtin_exit(t_executor *executor)
{
	int	exit_code;

	if (!executor || !executor->commands || !executor->commands->argv)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: no command to exit from\n");
		return ;
	}
	exit_code = executor->last_exit_code;
	if (executor->command_count <= 1 && isatty(STDIN_FILENO))
		ft_dprintf(STDOUT_FILENO, "exit\n");
	if (executor->commands->argv[1])
	{
		if (validate_exit_args(executor->commands->argv, &exit_code) != SUCCESS)
		{
			executor->last_exit_code = exit_code;
			return ;
		}
	}
	if (executor->command_count > 1)
	{
		executor->last_exit_code = exit_code;
		return ;
	}
	cleanup_and_exit(executor, exit_code);
}

static int	validate_exit_args(char **argv, int *exit_code)
{
	long long	result;
	char		*endptr;

	if (!is_valid_number_format(argv[1]) || is_overflow(argv[1])
		|| is_underflow(argv[1]))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", argv[1]);
		*exit_code = 2;
		return (SUCCESS);
	}
	if (argv[2])
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		*exit_code = 1;
		return (FAILURE);
	}
	result = ft_strtoll(argv[1], &endptr, 10);
	*exit_code = (int)result;
	return (SUCCESS);
}
