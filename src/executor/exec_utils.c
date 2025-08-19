/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:02:01 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/16 00:18:51 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"

void	count_commands(t_command *command, t_executor *executor)
{
	int	count;

	if (!executor || !command)
		return ;
	count = 0;
	executor->command_count = 0;
	while (command)
	{
		count++;
		command = command->next;
	}
	executor->command_count = count;
}

void	exec_external_command(t_executor *executor, t_command *command)
{
	char	*cmd_path;
	char	**envp;

	cmd_path = find_command_path(executor, command->argv[0]);
	envp = build_env_array(executor);
	if (!envp)
	{
		free(cmd_path);
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, command->argv, envp);
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", command->argv[0],
		strerror(errno));
	free(cmd_path);
	free_str_array(envp);
	exit(EXIT_CMD_NOT_FOUND);
}

void	add_envp_to_executor(t_executor *executor, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (add_env_variable(executor, envp[i]) == FAILURE)
		{
			print_error_stderr("Error adding environment variable\n");
			return ;
		}
		i++;
	}
}

static int	validate_single_command(t_executor *executor, t_command *command)
{
	char	*cmd_path;

	if (command->is_builtin)
		return (SUCCESS);
	if (!command->argv || !command->argv[0] || !command->argv[0][0])
	{
		ft_dprintf(STDERR_FILENO, "minishell: empty command\n");
		return (FAILURE);
	}
	cmd_path = find_command_path(executor, command->argv[0]);
	if (!cmd_path)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n",
			command->argv[0]);
		return (FAILURE);
	}
	free(cmd_path);
	return (SUCCESS);
}

int	validate_commands_exist(t_executor *executor, t_command *command)
{
	t_command	*current;
	int			has_errors;

	has_errors = 0;
	current = command;
	while (current)
	{
		if (validate_single_command(executor, current) == FAILURE)
			has_errors = 1;
		current = current->next;
	}
	if (has_errors)
		return (FAILURE);
	return (SUCCESS);
}
