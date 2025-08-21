/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:42:48 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/21 22:13:10 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	free_environment(t_executor *executor)
{
	t_env	*current;
	t_env	*next;

	current = executor->env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	executor->env_list = NULL;
}

void	free_executor(t_executor *executor)
{
	if (executor->pids)
		free(executor->pids);
	if (executor->logical_pwd)
		free(executor->logical_pwd);
	free_environment(executor);
	executor->command_count = 0;
	executor->current_command_index = 0;
}

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_inherited_state_for_child(t_executor *executor, t_command *commands)
{
	if (commands)
		free_commands(&commands);
	if (executor)
	{
		free_environment(executor);
		if (executor->pids)
			free(executor->pids);
		close_and_free_pipes(executor);
		executor->pids = NULL;
		executor->pipes = NULL;
	}
}
