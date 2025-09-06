/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:03:22 by cwong             #+#    #+#             */
/*   Updated: 2025/09/06 12:59:04 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	scan_quoted_word(char *input, int start, bool *has_dollar)
{
	int	len;

	len = 1;
	while (input[start + len] && input[start + len] != input[start])
	{
		if (input[start + len] == '$')
			*has_dollar = true;
		len++;
	}
	if (!input[start + len])
		return (-1);
	return (len);
}

int	parse_quote(char *input, int *i, t_quote *quote)
{
	int		len;
	bool	has_dollar;

	len = 1;
	has_dollar = false;
	len = scan_quoted_word(input, *i, &has_dollar);
	if (len < 0)
		return (-1);
	quote->quoted_word = ft_strndup(&input[*i + 1], len - 1);
	if (!quote->quoted_word)
		return (-1);
	if (input[*i] == '\"' && has_dollar && !quote->is_heredoc)
	{
		if (ft_strncmp(&input[*i], "\"$\"", 3) != 0)
		{
			if (expand_env_var(&quote->quoted_word, quote->executor) < 0)
			{
				free(quote->quoted_word);
				quote->quoted_word = NULL;
				return (-1);
			}
		}
	}
	*i += len + 1;
	return (0);
}

static int	parse_special(char *input, int *i, t_token **tokens,
	t_quote *quote)
{
	int				j;
	t_token_data	data;

	j = 1;
	if (input[*i + 1] == input[*i])
		j = 2;
	data.len = j;
	data.raw_value = NULL;
	if (ft_strncmp(&input[*i], "<<", 2) == 0)
		quote->is_heredoc = true;
	if (add_token(tokens, false, &input[*i], &data) < 0)
		return (free_tokens_input(tokens, NULL, 1));
	*i += j;
	return (0);
}

static int	handle_parsing(char *input, int *i, t_token **tokens,
		t_quote *quote)
{
	if (is_special(input[*i]))
	{
		if (parse_special(input, i, tokens, quote) < 0)
			return (-1);
	}
	else
	{
		if (parse_word(input, i, tokens, quote) < 0)
			return (-1);
	}
	return (0);
}

int	parse_input(char *input, t_token **tokens, t_executor *executor)
{
	int		i;
	t_quote	quote;

	quote.quoted_word = NULL;
	quote.executor = executor;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (input[i])
		{
			if (handle_parsing(input, &i, tokens, &quote) < 0)
				return (-1);
		}
	}
	return (0);
}
