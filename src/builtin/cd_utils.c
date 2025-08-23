/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:58:28 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/23 11:02:02 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"

static char	*resolve_path_components(char **components, int num_components);

void	cleanup_and_set_failure(t_executor *executor, int need_free,
		char *target_copy)
{
	if (need_free)
		free(target_copy);
	executor->last_exit_code = FAILURE;
}

char	*get_target_directory(t_executor *executor, t_command *command,
		int *need_free)
{
	char	*target_dir;
	char	*target_copy;

	*need_free = 0;
	if (!command->argv[1])
		target_dir = handle_cd_home(executor);
	else if (command->argv[1] && ft_strcmp(command->argv[1], "-") == 0)
		target_dir = handle_cd_dash(executor);
	else
		return (command->argv[1]);
	if (!target_dir)
		return (NULL);
	target_copy = ft_strdup(target_dir);
	if (!target_copy)
	{
		executor->last_exit_code = FAILURE;
		return (NULL);
	}
	*need_free = 1;
	return (target_copy);
}

char	*resolve_logical_path(char *current_logical_pwd, char *target)
{
	char	**components;
	char	*temp;
	char	*result;
	int		num_components;

	if (!current_logical_pwd || !target)
		return (NULL);
	if (target[0] == '/')
		return (ft_strdup(target));
	if (ft_strcmp(target, ".") == 0)
		return (ft_strdup(current_logical_pwd));
	temp = ft_strjoin(current_logical_pwd, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, target);
	free(temp);
	if (!result)
		return (NULL);
	components = ft_split(result, '/');
	free(result);
	if (!components)
		return (NULL);
	num_components = 0;
	while (components[num_components])
		num_components++;
	return (resolve_path_components(components, num_components));
}

static char	*resolve_path_components(char **components, int num_components)
{
	char	**resolved_parts;
	char	*result;
	int		i;
	int		j;

	resolved_parts = malloc(sizeof(char *) * (num_components + 1));
	if (!resolved_parts)
	{
		free_str_array(components);
		return (NULL);
	}
	j = 0;
	i = 0;
	while (i < num_components)
	{
		if (ft_strcmp(components[i], ".") == 0)
			;
		else if (ft_strcmp(components[i], "..") == 0 && j > 0)
		{
			free(resolved_parts[j - 1]);
			j--;
		}
		else if (ft_strcmp(components[i], "..") != 0)
		{
			resolved_parts[j] = ft_strdup(components[i]);
			if (!resolved_parts[j])
			{
				while (j > 0)
					free(resolved_parts[--j]);
				free(resolved_parts);
				free_str_array(components);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	resolved_parts[j] = NULL;
	result = join_path_components(resolved_parts, j);
	while (j > 0)
		free(resolved_parts[--j]);
	free(resolved_parts);
	free_str_array(components);
	return (result);
}

int	perform_directory_change(t_executor *executor, char *target_copy,
		int need_free)
{
	char	*new_logical_pwd;
	char	*test_cwd;

	if (update_oldpwd(executor) == FAILURE)
	{
		cleanup_and_set_failure(executor, need_free, target_copy);
		return (FAILURE);
	}
	new_logical_pwd = resolve_logical_path(executor->logical_pwd, target_copy);
	if (!new_logical_pwd)
	{
		cleanup_and_set_failure(executor, need_free, target_copy);
		return (FAILURE);
	}
	if (chdir(target_copy) != SUCCESS)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", target_copy,
			strerror(errno));
		free(new_logical_pwd);
		cleanup_and_set_failure(executor, need_free, target_copy);
		return (FAILURE);
	}
	test_cwd = getcwd(NULL, 0);
	if (!test_cwd)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: cd: error retrieving current directory: "
			"getcwd: cannot access parent directories: %s\n",
			strerror(errno));
	}
	else
		free(test_cwd);
	if (executor->logical_pwd)
		free(executor->logical_pwd);
	executor->logical_pwd = new_logical_pwd;
	if (need_free)
		free(target_copy);
	if (update_pwd(executor) == FAILURE)
	{
		executor->last_exit_code = FAILURE;
		return (FAILURE);
	}
	return (SUCCESS);
}
