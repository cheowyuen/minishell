/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:38:46 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/28 19:16:14 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

static int	is_hex_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

static int	parse_prefix(const char *str, int *i, int *sign, int base)
{
	*sign = 1;
	*i = 0;
	while (is_space(str[*i]))
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign = -1;
		(*i)++;
	}
	if (base == 16 && str[*i] == '0' && (str[*i + 1] == 'x' || str[*i
			+ 1] == 'X'))
	{
		*i += 2;
		return (1);
	}
	return (0);
}

static long long	convert_digits(const char *str, int *i, int base)
{
	long long	result;
	int			digit_value;

	result = 0;
	while (1)
	{
		if (base == 16 && is_hex_digit(str[*i]) != -1)
			digit_value = is_hex_digit(str[*i]);
		else if (base == 10 && ft_isdigit(str[*i]))
			digit_value = str[*i] - '0';
		else if (base == 8 && str[*i] >= '0' && str[*i] <= '7')
			digit_value = str[*i] - '0';
		else if (base == 2 && (str[*i] == '0' || str[*i] == '1'))
			digit_value = str[*i] - '0';
		else
			break ;
		result = result * base + digit_value;
		(*i)++;
	}
	return (result);
}

long long	ft_strtoll(const char *str, char **endptr, int base)
{
	int			i;
	int			sign;
	long long	result;

	parse_prefix(str, &i, &sign, base);
	result = convert_digits(str, &i, base);
	if (endptr)
		*endptr = (char *)&str[i];
	return (result * sign);
}
