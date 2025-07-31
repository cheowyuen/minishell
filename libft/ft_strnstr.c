/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:28:59 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/04/28 13:25:22 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	x;

	i = 0;
	if (!little[i])
		return ((char *)big);
	while (big[i] && i < len)
	{
		x = 0;
		while (big[i + x] == little[x] && (i + x) < len)
		{
			if (little[x + 1] == '\0')
			{
				return ((char *)&big[i]);
			}
			x++;
		}
		i++;
	}
	return (NULL);
}
