/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:26:44 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/04/28 10:47:14 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;
	int		dn;

	i = 0;
	dn = fd;
	if (!s)
		return ;
	while (s[i])
	{
		write(dn, &s[i], 1);
		i++;
	}
}
