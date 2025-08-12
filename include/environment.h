/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 09:33:28 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/11 10:52:08 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "executor.h"

int	set_oldpwd(t_executor *executor);
int	update_oldpwd(t_executor *executor);
int	update_pwd(t_executor *executor);

int	is_env_key_set(t_executor *executor, const char *key);

#endif
