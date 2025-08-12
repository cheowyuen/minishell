/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:58:28 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/12 11:35:01 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/builtin.h"
#include "../include/environment.h"

void	cleanup_and_set_failure(t_executor *executor, int need_free,
		char *target_copy)
{
	if (need_free)
		free(target_copy);
	executor->last_exit_code = FAILURE;
}

char	*get_target_directory(t_executor *executor, t_command *command,
		int *need_free)
{
	char	*target_dir;
	char	*target_copy;

	*need_free = 0;
	if (!command->argv[1])
		target_dir = handle_cd_home(executor);
	else if (command->argv[1] && ft_strcmp(command->argv[1], "-") == 0)
		target_dir = handle_cd_dash(executor);
	else
		return (command->argv[1]);
	if (!target_dir)
		return (NULL);
	target_copy = ft_strdup(target_dir);
	if (!target_copy)
	{
		executor->last_exit_code = FAILURE;
		return (NULL);
	}
	*need_free = 1;
	return (target_copy);
}

int	perform_directory_change(t_executor *executor, char *target_copy,
		int need_free)
{
	if (update_oldpwd(executor) == FAILURE)
	{
		cleanup_and_set_failure(executor, need_free, target_copy);
		return (FAILURE);
	}
	if (chdir(target_copy) != SUCCESS)
	{
		ft_dprintf(STDERR_FILENO, "bash: cd: %s: %s\n", target_copy,
			strerror(errno));
		cleanup_and_set_failure(executor, need_free, target_copy);
		return (FAILURE);
	}
	if (need_free)
		free(target_copy);
	if (update_pwd(executor) == FAILURE)
	{
		executor->last_exit_code = FAILURE;
		return (FAILURE);
	}
	return (SUCCESS);
}
