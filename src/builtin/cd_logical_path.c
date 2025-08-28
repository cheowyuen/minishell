/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_logical_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:58:28 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/28 19:47:41 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"

static char	*resolve_path_components(char **components, int num_components);

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


char	*join_path_components(char **resolved_parts, int count)
{
	char	*result;
	char	*temp;
	int		i;

	if (count == 0)
		return (ft_strdup("/"));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		temp = ft_strjoin(result, "/");
		free(result);
		if (!temp)
			return (NULL);
		result = ft_strjoin(temp, resolved_parts[i]);
		free(temp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}
