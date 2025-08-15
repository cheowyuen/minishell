/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:51:08 by cwong             #+#    #+#             */
/*   Updated: 2025/08/15 09:08:55 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

char	*ft_strndup(const char *src, int n)
{
	int		i;
	char	*duplicate;

	duplicate = (char *) malloc ((n + 1) * sizeof(char));
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

int	add_token(t_token **tokens, t_quote_type quote_type, char *content, int len)
{
	t_token	*new_token;

	new_token = ft_lstnew(quote_type, content, len);
	if (!new_token)
		return (-1);
	ft_lstadd_back(tokens, new_token);
	return (0);
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}
