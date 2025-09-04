/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:49:27 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/04 09:51:26 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	is_valid_number_format(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_overflow(const char *str)
{
	int			len;
	const char	*max_str = "9223372036854775807";
	int			i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		return (0);
	if (!str[i])
		return (1);
	len = ft_strlen(&str[i]);
	if (len > 19)
		return (1);
	if (len == 19)
	{
		if (ft_strncmp(&str[i], max_str, 19) > 0)
			return (1);
	}
	return (0);
}

int	is_underflow(const char *str)
{
	int			len;
	const char	*min_str = "9223372036854775808";
	int			i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '-')
		i++;
	else
		return (0);
	if (!str[i])
		return (1);
	len = ft_strlen(&str[i]);
	if (len > 19)
		return (1);
	if (len == 19)
	{
		if (ft_strncmp(&str[i], min_str, 19) > 0)
			return (1);
	}
	return (0);
}

void	cleanup_and_exit(t_executor *executor, int exit_code)
{
	rl_clear_history();
	if (executor->commands)
	{
		free_commands(&executor->commands);
		executor->commands = NULL;
	}
	if (executor->env_list)
		free_environment(executor);
	if (executor)
		free_executor(executor);
	exit(exit_code);
}
