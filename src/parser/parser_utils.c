/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:42:07 by cyuen             #+#    #+#             */
/*   Updated: 2025/08/30 22:00:45 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	*ft_strndup(const char *src, int n)
{
	int		i;
	char	*duplicate;

	if (!src)
		return (NULL);
	duplicate = (char *)malloc((n + 1) * sizeof(char));
	if (!duplicate)
		return (NULL);
	i = 0;
	while (i < n)
	{
		duplicate[i] = src[i];
		i++;
	}
	duplicate[i] = '\0';
	return (duplicate);
}

int	is_special(char c)
{
	if (!c)
		return (0);
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (!c)
		return (0);
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (!c)
		return (0);
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}
