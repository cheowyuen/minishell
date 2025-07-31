/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:18:21 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/04/26 22:01:21 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*newstr(const char *s, char c)
{
	size_t	i;
	size_t	len;
	char	*str;

	len = 0;
	i = 0;
	while (s[len] && s[len] != c)
		len++;
	str = malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static size_t	cwords(const char *s, char c)
{
	size_t	i;
	int		counter;

	counter = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		if (s[i] == '\0')
			return (counter);
		if (s[i] != c)
		{
			counter++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (counter);
}

static void	cleanstr(char **str, size_t i)
{
	while (i > 0)
	{
		i--;
		free(str[i]);
	}
	free(str);
}

static int	fillstr(char **strout, const char *s, char c)
{
	size_t	i;
	size_t	x;

	x = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			strout[x] = newstr(&s[i], c);
			if (strout[x] == NULL)
			{
				cleanstr(strout, x);
				return (0);
			}
			x++;
		}
		while (s[i] && s[i] != c)
			i++;
	}
	strout[x] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strout;

	if (s == NULL)
		return (NULL);
	strout = malloc((cwords(s, c) + 1) * sizeof(char *));
	if (strout == NULL)
		return (NULL);
	if (!fillstr(strout, s, c))
		return (NULL);
	return (strout);
}
