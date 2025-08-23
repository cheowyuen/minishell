/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:15 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/23 15:32:10 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "parser.h"

static int	is_valid_number(const char *str);
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
	if (executor->commands->argv[1])
	{
		if (validate_exit_args(executor->commands->argv, &exit_code) != SUCCESS)
			return ;
	}
	
	// If we're in a pipeline (command_count > 1), just set exit code and return
	// The child process will exit normally, allowing parent to clean up pipes
	if (executor->command_count > 1)
	{
		executor->last_exit_code = exit_code;
		return ;
	}
	
	// Only do full cleanup and exit if we're in the main shell (not in pipeline)
	cleanup_and_exit(executor, exit_code);
}

static int	is_valid_number(const char *str)
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
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	validate_exit_args(char **argv, int *exit_code)
{
	if (argv[2])
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		*exit_code = 1;
		return (SUCCESS);
	}
	if (!is_valid_number(argv[1]))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", argv[1]);
		*exit_code = 2;
		return (SUCCESS);
	}
	*exit_code = ft_atoi(argv[1]);
	return (SUCCESS);
}

static void	cleanup_and_exit(t_executor *executor, int exit_code)
{
	ft_dprintf(STDOUT_FILENO, "exit\n");
	// rl_clear_history();
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
