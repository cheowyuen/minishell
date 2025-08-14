/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:33:52 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:32:25 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	print_error_stderr(const char *msg)
{
	if (write(2, msg, strlen(msg)) == FAILURE_WRITE)
	{
		perror("write");
		return ;
	}
}

int	is_only_space_or_tab(char *str)
{
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}
