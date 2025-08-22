/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:48:32 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/22 16:45:49 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	builtin_pwd(t_executor *executor)
{
	char	*cwd;

	if (executor->logical_pwd)
	{
		printf("%s\n", executor->logical_pwd);
		executor->last_exit_code = SUCCESS;
	}
	else
	{
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
}
