/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_hex_digits.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:59:12 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/05/22 22:10:38 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_count_hex_digits(unsigned long num)
{
	int	count;

	count = 1;
	while (num >= 16)
	{
		num = num / 16;
		count++;
	}
	return (count);
}
