/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 09:31:36 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/07/12 09:50:28 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_putchar_fd_sp(int fd, char c)
{
	return (write(fd, &c, 1));
}

static int	ft_putstr_fd_sp(int fd, const char *s)
{
	int	len;

	len = 0;
	if (!s)
		return (write(fd, "(null)", 6));
	while (*s)
	{
		len += write(fd, s, 1);
		s++;
	}
	return (len);
}

static int	ft_putnbr_fd_sp(int fd, int n)
{
	char	buffer[12];
	int		i;
	int		neg;
	long	nb;

	i = 11;
	neg = 0;
	nb = n;
	buffer[i--] = '\0';
	if (nb < 0)
	{
		neg = 1;
		nb = -nb;
	}
	if (nb == 0)
		buffer[i--] = '0';
	while (nb > 0)
	{
		buffer[i--] = (nb % 10) + '0';
		nb /= 10;
	}
	if (neg)
		buffer[i--] = '-';
	return (ft_putstr_fd_sp(fd, &buffer[i + 1]));
}

static int	handle_format(int fd, char c, va_list args)
{
	if (c == 's')
		return (ft_putstr_fd_sp(fd, va_arg(args, char *)));
	if (c == 'd')
		return (ft_putnbr_fd_sp(fd, va_arg(args, int)));
	if (c == 'c')
		return (ft_putchar_fd_sp(fd, va_arg(args, int)));
	if (c == '%')
		return (ft_putchar_fd_sp(fd, '%'));
	return (ft_putchar_fd_sp(fd, c));
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	args;
	int		len;

	len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1))
		{
			format++;
			len += handle_format(fd, *format, args);
		}
		else
			len += ft_putchar_fd_sp(fd, *format);
		format++;
	}
	va_end(args);
	return (len);
}
