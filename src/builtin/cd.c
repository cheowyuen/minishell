/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:53:02 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/12 11:33:51 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/builtin.h"
#include "../include/environment.h"

void	builtin_cd(t_executor *executor, t_command *command)
{
	char	*target_copy;
	int		need_free;
	int		i;

	i = 0;
	while (command->argv[i])
		i++;
	if (i > 2)
	{
		print_error_stderr("bash: cd: too many arguments\n");
		executor->last_exit_code = FAILURE;
		return ;
	}
	target_copy = get_target_directory(executor, command, &need_free);
	if (!target_copy)
		return ;
	if (perform_directory_change(executor, target_copy, need_free) == SUCCESS)
		executor->last_exit_code = SUCCESS;
	else
		executor->last_exit_code = FAILURE;
}

char	*handle_cd_dash(t_executor *executor)
{
	char	*oldpwd;

	oldpwd = get_env_value(executor, "OLDPWD");
	if (!oldpwd)
	{
		ft_dprintf(STDERR_FILENO, "bash: cd: OLDPWD not set\n");
		executor->last_exit_code = FAILURE;
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

char	*handle_cd_home(t_executor *executor)
{
	char	*home;

	home = get_env_value(executor, "HOME");
	if (!home)
	{
		ft_dprintf(STDERR_FILENO, "bash: cd: HOME not set\n");
		executor->last_exit_code = FAILURE;
		return (NULL);
	}
	return (home);
}
