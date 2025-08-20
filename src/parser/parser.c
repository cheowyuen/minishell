/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:03:22 by cwong             #+#    #+#             */
/*   Updated: 2025/08/20 12:03:56 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	parse_quote(char *input, int *i, t_token **tokens, t_quote *quote)
{
	int				len;
	bool			has_dollar_sign;

	len = 1;
	has_dollar_sign = false;
	while (input[*i + len] && input[*i + len] != input[*i])
	{
		if (input[*i + len] == '$')
			has_dollar_sign = true;
		len++;
	}
	if (!input[*i + len])
		return (-1);
	if (input[*i] == '\"' && has_dollar_sign)
		quote->quote_type = DOUBLE_ENV;
	quote->quoted_word = ft_strndup(&input[*i + 1], len - 1);
	if (!quote->quoted_word)
		return (free_tokens_input(tokens, input));
	*i += len + 1;
	return (0);
}

static int	parse_word(char *input, int *i, t_token **tokens)
{
	char	*word;
	char	*joined;
	char	*temp;
	t_quote	quote;

	word = NULL;
	quote.quote_type = NONE;
	quote.quoted_word = NULL;	
	joined = ft_strdup("");
	if (!joined)
		return (free_tokens_input(tokens, input)); 
	while (input[*i] && !is_space(input[*i]) && !is_special(input[*i]))
	{
		if (is_quote(input[*i]))
		{
			if (parse_quote(input, i, tokens, &quote) < 0)
				return (-1);
			temp = ft_strjoin(joined, quote.quoted_word);
			free(joined);
			free(quote.quoted_word);
			quote.quoted_word = NULL;	
			if (!temp)
				return (free_tokens_input(tokens, input)); 
			joined = temp;
		}
		else
		{
			if (get_word(input, i, tokens, &word) < 0)
				return (-1);
			temp = ft_strjoin(joined, word);
			free(joined);
			free(word);
			word = NULL;
			if (!temp)
				return (free_tokens_input(tokens, input));
			joined = temp;
		}
	}
	if (add_token(tokens, quote.quote_type, joined, ft_strlen(joined)) < 0)
		return (free_tokens_input(tokens, input));
	return (0);
}

static int	parse_special(char *input, int *i, t_token **tokens)
{
	int				j;

	j = 1;
	if (input[*i + 1] == input[*i])
		j = 2;
	if (add_token(tokens, NONE, &input[*i], j) < 0)
		return (free_tokens_input(tokens, input));
	*i += j;
	return (0);
}

static int	handle_parsing(char *input, int *i, t_token **tokens)
{
	if (is_special(input[*i]))
	{
		if (parse_special(input, i, tokens) < 0)
			return (-1);
	}
	else
	{
		if (parse_word(input, i, tokens) < 0)
			return (-1);
	}
	return (0);
}

int	parse_input(char *input, t_token **tokens)
{
	int				i;

	i = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i])
		{
			if (handle_parsing(input, &i, tokens) < 0)
				return (-1);
		}
	}
	return (0);
}
