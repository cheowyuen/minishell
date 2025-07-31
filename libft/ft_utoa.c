/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:44:38 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/05/23 16:54:32 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strnlen(unsigned int num)
{
	int	len;

	len = 0;
	if (num == 0)
		return (1);
	while (num != 0)
	{
		num = num / 10;
		len++;
	}
	return (len);
}

char	*ft_utoa(unsigned int num)
{
	int		len;
	char	*str;

	len = ft_strnlen(num);
	str = malloc ((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (num == 0)
		str[0] = '0';
	while (num > 0)
	{
		len--;
		str[len] = (num % 10) + 48;
		num = num / 10;
	}
	return (str);
}
