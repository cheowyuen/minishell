/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:42:48 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/11 19:19:46 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.h"

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
	free_environment(executor);
	executor->command_count = 0;
	executor->current_command_index = 0;
	executor->last_exit_code = 0;
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

void	free_command(t_command *command)
{
	if (!command)
		return ;
	free_split(command->argv);
	free(command->infile);
	free(command->outfile);
	free(command->heredoc_delim);
	free(command);
}
