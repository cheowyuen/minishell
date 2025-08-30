/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:15 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 09:26:27 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "parser.h"

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
	if (executor->command_count <= 1 && isatty(STDIN_FILENO)) // Added check to not print exit when is not interactive
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

static int	validate_single_argument(const char *arg, int *exit_code,
		long long result)
{
	if (is_valid_number_format(arg) && ft_atoi(arg) < 0)
	{
		if (ft_atoi(arg) < INT_MAX && ft_atoi(arg) > INT_MIN)
		{
			*exit_code = ft_atoi(arg);
			return (FAILURE);
		}
	}
	if (arg[0] == '-' && result < 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", arg);
		*exit_code = 3;
		return (FAILURE);
	}
	*exit_code = (int)result;
	return (SUCCESS);
}

static int	validate_exit_args(char **argv, int *exit_code)
{
	long long	result;
	char		*endptr;

	if (!is_valid_number_format(argv[1]) || is_overflow(argv[1]))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", argv[1]);
		*exit_code = 2;
		return (SUCCESS);
	}
	result = ft_strtoll(argv[1], &endptr, 10);
	if (validate_single_argument(argv[1], exit_code, result) != SUCCESS)
		return (SUCCESS);
	if (argv[2])
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		*exit_code = 1;
		return (FAILURE);
	}
	return (SUCCESS);
}
