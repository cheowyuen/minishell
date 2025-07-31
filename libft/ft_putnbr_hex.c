/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:48:07 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/05/25 16:16:02 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_hex(unsigned long num, char *base)
{
	int	ret;

	if (num >= 16)
	{
		ret = ft_putnbr_hex(num / 16, base);
		if (ret < 0)
			return (-1);
	}
	if (ft_putchar_i_fd(base[num % 16], 1) < 0)
		return (-1);
	return (0);
}
