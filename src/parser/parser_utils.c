/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:51:08 by cwong             #+#    #+#             */
/*   Updated: 2025/08/20 11:47:34 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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

int	get_word(char *input, int *i, t_token **tokens, char **word)
{
	int	len;

	len = 0;
	while (input[*i + len] && !is_quote(input[*i + len])
		&& !is_space(input[*i + len]) && !is_special(input[*i + len]))
		len++;
	*word = ft_strndup(&input[*i], len);
	if (!*word)
		return (free_tokens_input(tokens, input));
	*i += len;
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
