/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:02:01 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/11 19:39:17 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/builtin.h"

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

static char	*search_in_paths(char **paths, char *command)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, command);
		if (!full_path)
		{
			free(temp);
			return (NULL);
		}
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(t_executor *executor, char *command)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	path_env = get_env_value(executor, "PATH");
	if (!path_env || !*path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, command);
	free_str_array(paths);
	return (result);
}

void	exec_external_command(t_executor *executor, t_command *command)
{
	char	*cmd_path;
	char	**envp;

	cmd_path = find_command_path(executor, command->argv[0]);
	if (!cmd_path)
	{
		ft_dprintf(STDERR_FILENO, "%s: command not found\n",
			command->argv[0]);
		exit(EXIT_CMD_NOT_FOUND);
	}
	envp = build_env_array(executor);
	if (!envp)
	{
		free(cmd_path);
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, command->argv, envp);
	perror("execve");
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
