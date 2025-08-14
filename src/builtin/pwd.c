/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:48:32 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:34:51 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	builtin_pwd(t_executor *executor)
{
	char	*cwd;

	if (executor->commands->argv[1])
	{
		print_error_stderr("pwd: too many arguments\n");
		executor->last_exit_code = FAILURE;
		return ;
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		executor->last_exit_code = FAILURE;
		return ;
	}
	printf("%s\n", cwd);
	free(cwd);
	executor->last_exit_code = SUCCESS;
}
