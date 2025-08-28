/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_logical_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:58:28 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/28 19:58:43 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"

char	*prepare_path_for_resolution(char *current_logical_pwd,
		char *target)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(current_logical_pwd, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, target);
	free(temp);
	return (result);
}

static int	process_path_component(char *component, char **resolved_parts,
		int *j)
{
	if (ft_strcmp(component, ".") == 0)
		return (0);
	if (ft_strcmp(component, "..") == 0 && *j > 0)
	{
		free(resolved_parts[*j - 1]);
		(*j)--;
		return (0);
	}
	if (ft_strcmp(component, "..") != 0)
	{
		resolved_parts[*j] = ft_strdup(component);
		if (!resolved_parts[*j])
			return (-1);
		(*j)++;
	}
	return (0);
}

static char	*finalize_path_resolution(char **resolved_parts, int j,
		char **components)
{
	char	*result;

	resolved_parts[j] = NULL;
	result = join_path_components(resolved_parts, j);
	while (j > 0)
		free(resolved_parts[--j]);
	free(resolved_parts);
	free_str_array(components);
	return (result);
}

char	*resolve_path_components(char **components, int num_components)
{
	char	**resolved_parts;
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
		if (process_path_component(components[i], resolved_parts, &j) == -1)
		{
			while (j > 0)
				free(resolved_parts[--j]);
			free(resolved_parts);
			free_str_array(components);
			return (NULL);
		}
		i++;
	}
	return (finalize_path_resolution(resolved_parts, j, components));
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
