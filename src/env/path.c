/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 09:00:02 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/15 18:11:39 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"
#include "minishell.h"
#include "utils.h"

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
	path_env = get_env_var(executor, "PATH");
	if (!path_env || !*path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, command);
	free_str_array(paths);
	return (result);
}
