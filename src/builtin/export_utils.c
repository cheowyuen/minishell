/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:12:03 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:34:40 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"

static int	alloc_and_add_var(t_executor *executor, const char *key,
				const char *value);

int	is_valid_identifier(const char *str)
{
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (*str != '=' && *str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

char	*get_env_var(t_executor *executor, const char *key)
{
	t_env	*env;

	if (!executor || !key || !executor->env_list)
		return (NULL);
	env = executor->env_list;
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0
			&& ft_strlen(env->key) == ft_strlen(key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	add_env_variable(t_executor *executor, const char *var)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
		return (FAILURE);
	key = ft_substr(var, 0, equal_sign - var);
	value = ft_strdup(equal_sign + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return (FAILURE);
	}
	if (add_var_to_list(executor, key, value) == FAILURE)
	{
		free(key);
		free(value);
		return (FAILURE);
	}
	free(key);
	free(value);
	return (SUCCESS);
}

int	add_var_to_list(t_executor *executor, const char *key, const char *value)
{
	t_env	*env;
	char	*new_val;

	env = executor->env_list;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			if (value)
			{
				new_val = ft_strdup(value);
				if (!new_val)
					return (FAILURE);
				free(env->value);
				env->value = new_val;
			}
			env->exported = EXPORTED;
			return (SUCCESS);
		}
		env = env->next;
	}
	return (alloc_and_add_var(executor, key, value));
}

static int	alloc_and_add_var(t_executor *executor, const char *key,
		const char *value)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (FAILURE);
	new_env->key = ft_strdup(key);
	if (value)
		new_env->value = ft_strdup(value);
	else
		new_env->value = NULL;
	new_env->exported = EXPORTED;
	if (!new_env->key || (value && !new_env->value))
	{
		free(new_env->key);
		free(new_env->value);
		free(new_env);
		return (FAILURE);
	}
	new_env->next = executor->env_list;
	executor->env_list = new_env;
	return (SUCCESS);
}
