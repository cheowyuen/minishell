/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:03:22 by cwong             #+#    #+#             */
/*   Updated: 2025/08/23 21:12:39 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	parse_quote(char *input, int *i, t_quote *quote)
{	
	int		len;
	bool	has_dollar_sign;

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
	quote->quoted_word = ft_strndup(&input[*i + 1], len - 1);
	if (!quote->quoted_word)
		return (-1);
	if (input[*i] == '\"' && has_dollar_sign)
		if (expand_env_var_new(&quote->quoted_word, quote->shell,
				quote->executor) < 0)
			return (-1);
	*i += len + 1;
	return (0);
}

static int	parse_word(char *input, int *i, t_token **tokens, t_quote *quote)
{
	char	*joined;

	joined = ft_strdup("");
	if (!joined)
		return (free_tokens_input(tokens, NULL, 1));
	while (input[*i] && !is_space(input[*i]) && !is_special(input[*i]))
	{
		if (is_quote(input[*i]))
		{
			if (handle_quoted_word(&joined, quote, input, i) < 0)
				return (free_tokens_input(tokens, NULL, 1));
		}
		else
		{
			if (handle_unquoted_word(&joined, quote, input, i) < 0)
				return (free_tokens_input(tokens, NULL, 1));
		}
	}
	if (add_token(tokens, quote->quote_type, joined, ft_strlen(joined)) < 0)
		return (free_tokens_input(tokens, NULL, 1));
	return (0);
}

static int	parse_special(char *input, int *i, t_token **tokens)
{
	int	j;

	j = 1;
	if (input[*i + 1] == input[*i])
		j = 2;
	if (add_token(tokens, NONE, &input[*i], j) < 0)
		return (free_tokens_input(tokens, NULL, 1));
	*i += j;
	return (0);
}

static int	handle_parsing(char *input, int *i, t_token **tokens,
		t_quote *quote)
{
	if (is_special(input[*i]))
	{
		if (parse_special(input, i, tokens) < 0)
			return (-1);
	}
	else
	{
		if (parse_word(input, i, tokens, quote) < 0)
			return (-1);
	}
	return (0);
}

int	parse_input(char *input, t_token **tokens, t_shell *shell,
		t_executor *executor)
{
	int		i;
	t_quote	quote;

	quote.quote_type = NONE;
	quote.quoted_word = NULL;
	quote.shell = shell;
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
