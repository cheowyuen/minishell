/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:53:02 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/21 22:58:07 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"

void	builtin_cd(t_executor *executor)
{
	char	*target_copy;
	int		need_free;
	int		i;

	i = 0;
	while (executor->commands->argv[i])
		i++;
	if (i > 2)
	{
		print_error_stderr("minishell: cd: too many arguments\n");
		executor->last_exit_code = FAILURE;
		return ;
	}
	target_copy = get_target_directory(executor, executor->commands,
			&need_free);
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

char	*join_path_components(char **resolved_parts, int count)
{
	char	*result;
	char	*temp;
	int		i;

	if (count == 0)
		return (ft_strdup("/"));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		temp = ft_strjoin(result, "/");
		free(result);
		if (!temp)
			return (NULL);
		result = ft_strjoin(temp, resolved_parts[i]);
		free(temp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}
