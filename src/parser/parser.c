/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:03:22 by cwong             #+#    #+#             */
/*   Updated: 2025/09/02 16:53:25 by lsurco-t         ###   ########.fr       */
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
		if (ft_strncmp(&input[*i], "\"$\"", 3) != 0)
			if (expand_env_var(&quote->quoted_word, quote->executor) < 0)
			{
				free(quote->quoted_word);
				quote->quoted_word = NULL;
				return (-1);
			}
	*i += len + 1;
	return (0);
}

static int	parse_word(char *input, int *i, t_token **tokens, t_quote *quote)
{
	char	*joined;
	bool	is_quoted;
	bool	has_unquoted_vars;
	int		start;
	int		j;
	int		starting_pos;
	bool	has_quotes;
	char	*trimmed;
	char	*temp;

	//	bool	has_spaces;
	quote->raw_value = NULL;
	joined = ft_strdup("");
	if (!joined)
		return (free_tokens_input(tokens, NULL, 1));
	is_quoted = false;
	has_unquoted_vars = false;
	starting_pos = *i;
	has_quotes = false;
	// has_spaces = false;
	temp = NULL;
	while (input[*i] && !is_space(input[*i]) && !is_special(input[*i]))
	{
		if (is_quote(input[*i]))
		{
			if (*i == starting_pos)
				has_quotes = true;
			is_quoted = true;
			if (handle_quoted_word(&joined, quote, input, i) < 0)
			{
				free(joined);
				return (free_tokens_input(tokens, NULL, 1));
			}
			if (has_quotes && joined[0] != '\0')
			{
				trimmed = ft_strtrim(joined, " ");
				if (!trimmed)
				{
					free(joined);
					return (free_tokens_input(tokens, NULL, 1));
				}
				if (trimmed[0] == '\0')
				{
					temp = ft_substr(joined, 0, ft_strlen(joined) - 1);
					if (!temp)
					{
						free(joined);
						return (free_tokens_input(tokens, NULL, 1));
					}
				}
				free(trimmed);
				has_quotes = false;
			}
		}
		else
		{
			start = *i;
			if (handle_unquoted_word(&joined, quote, input, i) < 0)
			{
				free(joined);
				return (free_tokens_input(tokens, NULL, 1));
			}
			j = start;
			while (j < *i)
			{
				if (input[j] == '$')
				{
					has_unquoted_vars = true;
					break ;
				}
				j++;
			}
		}
	}
	if (temp)
	{
		if (has_unquoted_vars)
		{
			if (add_tokens_with_splitting(tokens, is_quoted, temp,
					quote->raw_value) < 0)
			{
				free(joined);
				free(temp);
				return (free_tokens_input(tokens, NULL, 1));
			}
		}
		free(temp);
	}
	// Use word splitting for unquoted tokens that might contain expanded variables
	if (add_tokens_with_splitting(tokens, is_quoted && !has_unquoted_vars,
			joined, quote->raw_value) < 0)
	{
		free(joined);
		return (free_tokens_input(tokens, NULL, 1));
	}
	free(joined);
	free(quote->raw_value);
	return (0);
}

static int	parse_special(char *input, int *i, t_token **tokens)
{
	int				j;
	t_token_data	data;

	j = 1;
	if (input[*i + 1] == input[*i])
		j = 2;
	data.len = j;
	data.raw_value = NULL;
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
