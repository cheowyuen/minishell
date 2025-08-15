/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:46:52 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:34:46 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	handle_export_argument(t_executor *executor, const char *arg,
				char *equal_sign);
static int	mark_as_exported(t_executor *executor, const char *arg);
static int	check_valid_identifier(char *arg, char *equal_sign, char *key);
static void	list_exported_variables(t_executor *executor);

void	builtin_export(t_executor *executor)
{
	int		i;
	char	*equal_sign;
	char	*key;

	i = 1;
	list_exported_variables(executor);
	while (executor->commands->argv[i])
	{
		equal_sign = ft_strchr(executor->commands->argv[i], '=');
		key = NULL;
		if (check_valid_identifier(executor->commands->argv[i], equal_sign,
				key) == FAILURE)
		{
			executor->last_exit_code = FAILURE;
			return ;
		}
		if (handle_export_argument(executor, executor->commands->argv[i],
				equal_sign) == FAILURE)
		{
			executor->last_exit_code = FAILURE;
			return ;
		}
		i++;
	}
	executor->last_exit_code = SUCCESS;
}

static int	check_valid_identifier(char *arg, char *equal_sign, char *key)
{
	int	valid;

	valid = 0;
	if (equal_sign)
	{
		key = ft_substr(arg, 0, equal_sign - arg);
		if (!key)
			return (FAILURE);
		valid = is_valid_identifier(key);
	}
	else
		valid = is_valid_identifier(arg);
	if (!valid)
	{
		if (key)
			ft_dprintf(2, "bash: export: `%s': not a valid identifier\n", key);
		else
			ft_dprintf(2, "bash: export: `%s': not a valid identifier\n", arg);
		if (key)
			free(key);
		return (FAILURE);
	}
	if (key)
		free(key);
	return (SUCCESS);
}

static int	handle_export_argument(t_executor *executor, const char *arg,
		char *equal_sign)
{
	if (equal_sign)
	{
		if (add_env_variable(executor, arg) == FAILURE)
		{
			print_error_stderr("bash: export: failed to add env variable\n");
			return (FAILURE);
		}
	}
	else
	{
		if (mark_as_exported(executor, arg) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

static int	mark_as_exported(t_executor *executor, const char *arg)
{
	t_env	*env;

	env = executor->env_list;
	while (env)
	{
		if (ft_strcmp(env->key, arg) == 0)
		{
			env->exported = EXPORTED;
			return (SUCCESS);
		}
		env = env->next;
	}
	return (add_var_to_list(executor, arg, NULL));
}

static void	list_exported_variables(t_executor *executor)
{
	t_env	*env;

	env = executor->env_list;
	if (!executor->commands->argv[1])
	{
		while (env)
		{
			if (env->exported)
			{
				if (env->value != NULL)
					printf("declare -x %s=\"%s\"\n", env->key, env->value);
				else
					printf("declare -x %s\n", env->key);
			}
			env = env->next;
		}
		executor->last_exit_code = SUCCESS;
		return ;
	}
}
