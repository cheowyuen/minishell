/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:48:11 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/05/27 10:38:42 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	conversion_check(char c, va_list args)
{
	int	return_value;

	return_value = 0;
	if (c == 'c' || c == '%' || c == 's')
		return_value = str_or_ch(c, args);
	else if (c == 'x' || c == 'X' || c == 'p')
		return_value = num_hex(c, args);
	else if (c == 'd' || c == 'i' || c == 'u')
		return_value = num_gen(c, args);
	else
		return (-1);
	if (return_value < 0)
		return (-1);
	return (return_value);
}

static int	error_ret(va_list args)
{
	va_end(args);
	return (-1);
}

int	ft_printf(const char *formats, ...)
{
	va_list	args;
	int		len;
	int		ret;

	len = 0;
	va_start(args, formats);
	while (*formats)
	{
		if (*formats == '%' && *(formats + 1))
		{
			formats++;
			ret = conversion_check(*formats++, args);
			if (ret < 0)
				return (error_ret(args));
			len += ret;
		}
		else
		{
			if (write(1, formats++, 1) < 0)
				return (error_ret(args));
			len++;
		}
	}
	va_end(args);
	return (len);
}
