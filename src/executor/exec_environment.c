/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:01:31 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:35:52 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	count_exported_variables(t_env *env_list);
static int	add_env_to_array(char **env_array, t_env *env, int i);
static char	*create_env_string(t_env *env);

char	**build_env_array(t_executor *executor)
{
	t_env	*env;
	char	**env_array;
	int		count;
	int		i;

	count = count_exported_variables(executor->env_list);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	env = executor->env_list;
	i = 0;
	while (env)
	{
		if (env->exported && add_env_to_array(env_array, env, i) == SUCCESS)
			i++;
		else if (env->exported)
		{
			free_str_array(env_array);
			return (NULL);
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

static int	add_env_to_array(char **env_array, t_env *env, int i)
{
	env_array[i] = create_env_string(env);
	if (!env_array[i])
		return (FAILURE);
	return (SUCCESS);
}

static int	count_exported_variables(t_env *env_list)
{
	t_env	*env;
	int		count;

	env = env_list;
	count = 0;
	while (env)
	{
		if (env->exported)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*create_env_string(t_env *env)
{
	char	*env_str;
	char	*temp;

	if (!env->value)
		return (ft_strdup(env->key));
	temp = ft_strjoin(env->key, "=");
	if (!temp)
		return (NULL);
	env_str = ft_strjoin(temp, env->value);
	if (!env_str)
	{
		free(temp);
		return (NULL);
	}
	free(temp);
	return (env_str);
}
