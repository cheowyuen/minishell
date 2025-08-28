/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:53:02 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/28 19:39:02 by lsurco-t         ###   ########.fr       */
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

static int	execute_chdir(t_executor *executor, char *target_copy,
		char *new_logical_pwd, int need_free)
{
	char	*test_cwd;

	if (chdir(target_copy) != SUCCESS)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", target_copy,
			strerror(errno));
		free(new_logical_pwd);
		cleanup_and_set_failure(executor, need_free, target_copy);
		return (FAILURE);
	}
	test_cwd = getcwd(NULL, 0);
	if (!test_cwd)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: cd: error retrieving current directory: "
			"getcwd: cannot access parent directories: %s\n",
			strerror(errno));
	}
	else
		free(test_cwd);
	return (SUCCESS);
}

static int	finalize_directory_change(t_executor *executor,
		char *new_logical_pwd, char *target_copy, int need_free)
{
	if (executor->logical_pwd)
		free(executor->logical_pwd);
	executor->logical_pwd = new_logical_pwd;
	if (need_free)
		free(target_copy);
	if (update_pwd(executor) == FAILURE)
	{
		executor->last_exit_code = FAILURE;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	perform_directory_change(t_executor *executor, char *target_copy,
		int need_free)
{
	char	*new_logical_pwd;

	if (update_oldpwd(executor) == FAILURE)
	{
		cleanup_and_set_failure(executor, need_free, target_copy);
		return (FAILURE);
	}
	new_logical_pwd = resolve_logical_path(executor->logical_pwd, target_copy);
	if (!new_logical_pwd)
	{
		cleanup_and_set_failure(executor, need_free, target_copy);
		return (FAILURE);
	}
	if (execute_chdir(executor, target_copy, new_logical_pwd,
			need_free) == FAILURE)
		return (FAILURE);
	return (finalize_directory_change(executor, new_logical_pwd, target_copy,
			need_free));
}
