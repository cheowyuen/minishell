/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 19:53:43 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/04/28 10:45:16 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*str;
	size_t	memsize;

	if (nmemb == 0 || size == 0)
	{
		str = malloc(0);
		return (str);
	}
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	memsize = nmemb * size;
	str = malloc(memsize);
	if (str == NULL)
		return (NULL);
	ft_memset(str, 0, memsize);
	return (str);
}
