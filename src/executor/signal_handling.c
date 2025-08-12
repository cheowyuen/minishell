/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:01:09 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/08 17:51:05 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

void	handle_sigint(int sig)
{
	(void)sig;
	ft_dprintf(STDOUT_FILENO, "\n");
}

void	handle_sigquit(int sig)
{
	(void)sig;
	ft_dprintf(STDOUT_FILENO, "Quit (core dumped)\n");
}
