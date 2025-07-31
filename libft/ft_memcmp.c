/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:22:07 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/04/28 10:44:53 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*ch1;
	unsigned char	*ch2;

	if (n == 0)
		return (0);
	i = 0;
	ch1 = (unsigned char *)s1;
	ch2 = (unsigned char *)s2;
	while (i < n)
	{
		if (ch1[i] != ch2[i])
		{
			return (ch1[i] - ch2[i]);
		}
		i++;
	}
	return (0);
}
