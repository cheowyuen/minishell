/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:18:09 by cheowyuenwo       #+#    #+#             */
/*   Updated: 2025/09/05 10:10:48 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	handle_dollar_single_quote(char *input, int *i, char **word)
{
	int	len;

	len = 2;
	while (input[*i + len] && input[*i + len] != '\'')
		len++;
	if (input[*i + len] == '\'')
		len++;
	*word = ft_strndup(&input[*i + 2], len - 3);
	if (!*word)
		return (-1);
	*i += len;
	return (0);
}

static int	get_word_length(char *input, int start, bool *has_dollar)
{
	int	len;

	len = 0;
	while (input[start + len] && !is_quote(input[start + len])
		&& !is_space(input[start + len]) && !is_special(input[start + len]))
	{
		if (input[start + len] == '$')
			*has_dollar = true;
		len++;
	}
	return (len);
}

int	get_word(char *input, int *i, t_quote *quote, char **word)
{
	int		len;
	bool	has_dollar;

	has_dollar = false;
	if (input[*i] == '$' && input[*i + 1] == '\'')
		return (handle_dollar_single_quote(input, i, word));
	len = get_word_length(input, *i, &has_dollar);
	*word = ft_strndup(&input[*i], len);
	if (!*word)
		return (-1);
	if ((*word)[0] == '$')
	{
		if (quote->raw_value)
			free(quote->raw_value);
		quote->raw_value = ft_strndup(&input[*i], len);
		if (!quote->raw_value)
			return (-1);
	}
	if (expand_word_if_needed(word, has_dollar, quote->executor) < 0)
		return (-1);
	*i += len;
	return (0);
}

int	handle_quoted_word(char **joined, t_quote *quote, char *input, int *i)
{
	char	*temp;

	if (parse_quote(input, i, quote) < 0)
		return (-1);
	temp = ft_strjoin(*joined, quote->quoted_word);
	free(*joined);
	free(quote->quoted_word);
	quote->quoted_word = NULL;
	if (!temp)
		return (-1);
	*joined = temp;
	return (0);
}

int	handle_unquoted_word(char **joined, t_quote *quote, char *input, int *i)
{
	char	*word;
	char	*temp;

	word = NULL;
	if (get_word(input, i, quote, &word) < 0)
		return (-1);
	temp = ft_strjoin(*joined, word);
	free(*joined);
	free(word);
	if (!temp)
		return (-1);
	*joined = temp;
	return (0);
}
