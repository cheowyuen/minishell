/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:33:52 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/09 09:29:50 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.h"

void	print_error_stderr(const char *msg)
{
	if (write(2, msg, strlen(msg)) == FAILURE_WRITE)
	{
		perror("write");
		return ;
	}
}
