/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuen <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:18:09 by cheowyuenwo       #+#    #+#             */
/*   Updated: 2025/08/31 08:42:20 by cyuen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

int	get_word_heredoc(char *input, int *i, t_quote *quote, char **word)
{
	int		len;
	bool	has_dollar_sign;

	len = 0;
	has_dollar_sign = false;
	while (input[*i + len])
	{
		if (input[*i + len] == '$')
		{
			has_dollar_sign = true;
		}
		len++;
	}
	*word = ft_strndup(&input[*i], len);
	if (!*word)
		return (-1);
	if (has_dollar_sign)
	{
		if (expand_env_var(word, quote->executor) < 0)
			return (-1);
	}
	*i += len;
	return (0);
}

int	get_word(char *input, int *i, t_quote *quote, char **word)
{
	int		len;
	bool	has_dollar_sign;

	len = 0;
	has_dollar_sign = false;
	// Check for ANSI-C quoting pattern $'...'
	if (input[*i] == '$' && input[*i + 1] == '\'')
	{
		// Skip the $' and find the closing quote
		len = 2;
		while (input[*i + len] && input[*i + len] != '\'')
			len++;
		if (input[*i + len] == '\'')
			len++; // Include closing quote
		*word = ft_strndup(&input[*i + 2], len - 3); // Extract content without $' and '
		if (!*word)
			return (-1);
		*i += len;
		return (0);
	}
	while (input[*i + len] && !is_quote(input[*i + len]) && !is_space(input[*i
				+ len]) && !is_special(input[*i + len]))
	{
		if (input[*i + len] == '$')
		{
			has_dollar_sign = true;
		}
		len++;
	}
	*word = ft_strndup(&input[*i], len);
	if (!*word)
		return (-1);
	if ((*word)[0] == '$')
    {
		quote->raw_value = ft_strndup(&input[*i], len);
		if (!quote->raw_value)
			return (-1);
	}
	if (has_dollar_sign)
	{
		if (expand_env_var(word, quote->executor) < 0)
			return (-1);
	}
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
	quote->is_heredoc = false;
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
