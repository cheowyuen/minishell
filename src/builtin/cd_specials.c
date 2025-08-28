/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_specials.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:32:09 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/28 19:32:38 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"

char	*handle_cd_dash(t_executor *executor)
{
	char	*oldpwd;

	oldpwd = get_env_var(executor, "OLDPWD");
	if (!oldpwd)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
		executor->last_exit_code = FAILURE;
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

char	*handle_cd_home(t_executor *executor)
{
	char	*home;

	home = get_env_var(executor, "HOME");
	if (!home)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
		executor->last_exit_code = FAILURE;
		return (NULL);
	}
	return (home);
}
