/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:52:02 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/05/27 10:38:50 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	num_gen(char c, va_list args)
{
	int				num_int_dec;
	unsigned int	num_un_dec;
	char			*str;
	int				ret;

	if (c == 'd' || c == 'i')
	{
		num_int_dec = va_arg(args, int);
		str = ft_itoa(num_int_dec);
	}
	else if (c == 'u')
	{
		num_un_dec = va_arg(args, unsigned int);
		str = ft_utoa(num_un_dec);
	}
	else
		return (-1);
	if (!str)
		return (-1);
	ret = ft_putstr_i_fd(str, 1);
	free(str);
	return (ret);
}

int	num_hex(char c, va_list args)
{
	unsigned long	num;
	char			*hex_base;
	int				ret;

	hex_base = "0123456789abcdef";
	if (c == 'p')
	{
		num = (unsigned long)va_arg(args, void *);
		if (num == 0)
			return (ft_putstr_i_fd("(nil)", 1));
		ret = ft_putstr_i_fd("0x", 1);
		if (ret < 0)
			return (-1);
		if (ft_putnbr_hex(num, hex_base) < 0)
			return (-1);
		return (ret + ft_count_hex_digits(num));
	}
	num = va_arg(args, unsigned int);
	if (c == 'X')
		hex_base = "0123456789ABCDEF";
	if (ft_putnbr_hex(num, hex_base) < 0)
		return (-1);
	return (ft_count_hex_digits(num));
}

int	str_or_ch(char c, va_list args)
{
	char	*str;
	char	ch;

	if (c == 'c')
	{
		ch = va_arg(args, int);
		return (ft_putchar_i_fd(ch, 1));
	}
	else if (c == '%')
	{
		return (ft_putchar_i_fd('%', 1));
	}
	else if (c == 's')
	{
		str = va_arg(args, char *);
		if (!str)
			str = "(null)";
		return (ft_putstr_i_fd(str, 1));
	}
	return (-1);
}
