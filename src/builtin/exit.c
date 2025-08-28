/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:15 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/28 23:20:14 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "parser.h"

static int	validate_numeric_argument(const char *arg, int *exit_code);
static int	validate_exit_args(char **argv, int *exit_code);
static void	cleanup_and_exit(t_executor *executor, int exit_code);

void	builtin_exit(t_executor *executor)
{
	int	exit_code;

	if (!executor || !executor->commands || !executor->commands->argv)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: no command to exit from\n");
		return ;
	}
	exit_code = executor->last_exit_code;
	if (executor->command_count <= 1)
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

static int	is_valid_number_format(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	validate_numeric_argument(const char *arg, int *exit_code)
{
	long long	result;
	char		*endptr;
	
	result = ft_strtoll(arg, &endptr, 10);
	if (arg[0] == '-' && result < 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", arg);
		*exit_code = 3;
		return (SUCCESS);
	}
	*exit_code = (int)result;
	return (SUCCESS);
}

static int	validate_exit_args(char **argv, int *exit_code)
{
	int			i;

	i = 0;
	if (is_valid_number_format(argv[1]) && ft_atoi(argv[1]) < 0)
	{
		if (argv[1][i] == '+' || argv[1][i] == '-')
			i++;
		if (!is_valid_number_format(argv[1]) || ft_strlen(&argv[1][i]) > 19)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: exit: %s: numeric argument required\n", argv[1]);
			*exit_code = 2;
			return (SUCCESS);
		}
		*exit_code = ft_atoi(argv[1]);
		return (SUCCESS);
	}
	if (argv[2])
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		*exit_code = 1;
		return (FAILURE);
	}
	return (validate_numeric_argument(argv[1], exit_code));
}

static void	cleanup_and_exit(t_executor *executor, int exit_code)
{
	rl_clear_history();
	if (executor->commands)
	{
		free_commands(&executor->commands);
		executor->commands = NULL;
	}
	if (executor->env_list)
		free_environment(executor);
	if (executor)
		free_executor(executor);
	exit(exit_code);
}
