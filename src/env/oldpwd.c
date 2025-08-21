/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oldpwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 09:00:18 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/21 22:37:57 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	set_oldpwd(t_executor *executor)
{
	char	*oldpwd;

	oldpwd = get_env_var(executor, "OLDPWD");
	if (oldpwd)
	{
		printf("%s\n", oldpwd);
		executor->last_exit_code = SUCCESS;
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
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
		return (SUCCESS);
	if (add_var_to_list(executor, "OLDPWD", current_pwd) == FAILURE)
	{
		free(current_pwd);
		return (FAILURE);
	}
	free(current_pwd);
	return (SUCCESS);
}

int	update_pwd(t_executor *executor)
{
	char	*pwd_to_use;

	if (executor->logical_pwd)
	{
		pwd_to_use = executor->logical_pwd;
		if (add_var_to_list(executor, "PWD", pwd_to_use) == FAILURE)
			return (FAILURE);
	}
	else
	{
		pwd_to_use = getcwd(NULL, 0);
		if (!pwd_to_use)
			return (SUCCESS);
		if (add_var_to_list(executor, "PWD", pwd_to_use) == FAILURE)
		{
			free(pwd_to_use);
			return (FAILURE);
		}
		free(pwd_to_use);
	}
	return (SUCCESS);
}
