/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:52:16 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/04/28 10:43:33 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	i;
	char	*strim;

	if (set == NULL || s1 == NULL)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	strim = malloc((end - start + 1) * sizeof(char));
	if (strim == NULL)
		return (NULL);
	i = 0;
	while (start < end)
	{
		strim[i] = s1[start];
		i++;
		start++;
	}
	strim[i] = '\0';
	return (strim);
}
