/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_i_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:26:44 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/05/25 16:28:05 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_i_fd(char *s, int fd)
{
	ssize_t		len;

	len = 0;
	if (!s)
		return (-1);
	len = write(fd, s, ft_strlen(s));
	if (len < 0)
		return (-1);
	return ((int)len);
}
