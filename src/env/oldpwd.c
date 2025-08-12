/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oldpwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 09:00:18 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/11 15:42:30 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/builtin.h"

int	set_oldpwd(t_executor *executor)
{
	char	*oldpwd;

	oldpwd = get_env_value(executor, "OLDPWD");
	if (oldpwd)
	{
		printf("%s\n", oldpwd);
		executor->last_exit_code = SUCCESS;
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "cd: OLDPWD not set\n");
		executor->last_exit_code = FAILURE;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	update_oldpwd(t_executor *executor)
{
	char	*current_pwd;

	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		perror("getcwd");
		return (FAILURE);
	}
	if (add_env_to_list(executor, "OLDPWD", current_pwd) == FAILURE)
	{
		free(current_pwd);
		return (FAILURE);
	}
	free(current_pwd);
	return (SUCCESS);
}

int	update_pwd(t_executor *executor)
{
	char	*current_pwd;

	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		perror("getcwd");
		return (FAILURE);
	}
	if (add_env_to_list(executor, "PWD", current_pwd) == FAILURE)
	{
		free(current_pwd);
		return (FAILURE);
	}
	free(current_pwd);
	return (SUCCESS);
}
