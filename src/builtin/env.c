/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:47:08 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:34:25 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	builtin_env(t_executor *executor)
{
	t_env	*env;

	if (executor->commands->argv[1])
	{
		print_error_stderr("env: run with no options or arguments\n");
		executor->last_exit_code = FAILURE;
		return ;
	}
	env = executor->env_list;
	while (env)
	{
		if (env->value != NULL)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	executor->last_exit_code = SUCCESS;
}
