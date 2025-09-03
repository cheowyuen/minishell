/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:02 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/03 12:23:25 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	remove_env_variable(t_executor *executor, const char *key);

void	builtin_unset(t_executor *executor)
{
	int		i;
	char	*key;

	i = 1;
	while (executor->commands->argv[i])
	{
		key = executor->commands->argv[i];
		if (is_env_key_set(executor, key))
			remove_env_variable(executor, key);
		i++;
	}
	executor->last_exit_code = SUCCESS;
}

static int	remove_env_variable(t_executor *executor, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = executor->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				executor->env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (SUCCESS);
		}
		prev = current;
		current = current->next;
	}
	return (FAILURE);
}
