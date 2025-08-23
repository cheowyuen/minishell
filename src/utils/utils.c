/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:33:52 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/23 11:16:03 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	print_error_stderr(const char *msg)
{
	if (write(2, msg, strlen(msg)) == FAILURE_WRITE)
	{
		perror("write");
		return ;
	}
}

int	is_only_space_or_tab(char *str)
{
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

int	free_input_failure(char *input)
{
	free(input);
	return (1);
}

void	sort_env_list_alpha(t_env **env_list)
{
	t_env	*current;
	t_env	*next;
	char	*temp;

	if (!env_list || !*env_list)
		return ;
	current = *env_list;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (ft_strcmp(current->key, next->key) > 0)
			{
				temp = current->key;
				current->key = next->key;
				next->key = temp;
			}
			next = next->next;
		}
		current = current->next;
	}
}
