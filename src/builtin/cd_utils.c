/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:32:09 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/05 10:27:47 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

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

void	cleanup_and_set_failure(t_executor *executor, int need_free,
		char *target_copy)
{
	if (need_free)
		free(target_copy);
	executor->last_exit_code = FAILURE;
}

char	*resolve_logical_path(char *current_logical_pwd, char *target)
{
	char	**components;
	char	*result;
	int		num_components;

	if (!current_logical_pwd || !target)
		return (NULL);
	if (target[0] == '/')
		return (ft_strdup(target));
	if (ft_strcmp(target, ".") == 0)
		return (ft_strdup(current_logical_pwd));
	result = prepare_path_for_resolution(current_logical_pwd, target);
	if (!result)
		return (NULL);
	components = ft_split(result, '/');
	free(result);
	if (!components)
		return (NULL);
	num_components = 0;
	while (components[num_components])
		num_components++;
	return (resolve_path_components(components, num_components));
}
