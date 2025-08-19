/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:03:22 by cwong             #+#    #+#             */
/*   Updated: 2025/08/19 11:33:40 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	parse_word(char *input, int *i, t_token **tokens)
{
	int	len;

	len = 0;
	while (input[*i + len] && !is_quote(input[*i + len])
		&& !is_space(input[*i + len]) && !is_special(input[*i + len]))
		len++;
	if (add_token(tokens, NONE, &input[*i], len) < 0)
		return (free_tokens_input(tokens, input));
	*i += len;
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

static int	parse_quote(char *input, int *i, t_token **tokens)
{
	int				len;
	t_quote_type	quote_type;
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
	if (input[*i] == '\'')
		quote_type = SINGLE;
	else if (has_dollar_sign)
		quote_type = DOUBLE_ENV;
	else
		quote_type = DOUBLE;
	if (add_token(tokens, quote_type, &input[*i + 1], len - 1) < 0)
		return (free_tokens_input(tokens, input));
	*i += len + 1;
	return (0);
}

static int	handle_parsing(char *input, int *i, t_token **tokens)
{
	if (is_quote(input[*i]))
	{
		if (parse_quote(input, i, tokens) < 0)
			return (-1);
	}
	else if (is_special(input[*i]))
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
		if (handle_parsing(input, &i, tokens) < 0)
			return (-1);
	}
	return (0);
}
