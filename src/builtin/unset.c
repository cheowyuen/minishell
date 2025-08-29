/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:02 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/29 11:17:53 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	remove_env_variable(t_executor *executor, const char *key);
static int	is_valid_unset_identifier(const char *str);

void	builtin_unset(t_executor *executor)
{
	int		i;
	char	*key;
	int		had_invalid;

	had_invalid = 0;
	i = 1;
	while (executor->commands->argv[i])
	{
		key = executor->commands->argv[i];
		if (!is_valid_unset_identifier(key))
		{
			ft_dprintf(STDERR_FILENO, "unset: %s: invalid parameter name\n",
				key);
			had_invalid = 1;
		}
		else if (is_env_key_set(executor, key))
			remove_env_variable(executor, key);
		i++;
	}
	if (had_invalid)
		executor->last_exit_code = 2;
	else
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

static int	is_valid_first_char(char c)
{
	const char	valid_first[] = "+=?$#!%&*()";
	int			i;

	if (ft_isalpha(c) || c == '_')
		return (1);
	i = 0;
	while (valid_first[i])
	{
		if (c == valid_first[i])
			return (1);
		i++;
	}
	return (0);
}

static int	is_valid_other_char(char c)
{
	const char	valid_chars[] = ";.+=}{-*#@!^~\\";
	int			i;

	if (ft_isalnum(c) || c == '_')
		return (1);
	i = 0;
	while (valid_chars[i])
	{
		if (c == valid_chars[i])
			return (1);
		i++;
	}
	return (0);
}

static int	is_valid_unset_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!is_valid_first_char(str[0]))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!is_valid_other_char(str[i]))
			return (0);
		i++;
	}
	return (1);
}
