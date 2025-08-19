/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:03:22 by cwong             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/08/19 11:33:40 by cwong            ###   ########.fr       */
=======
/*   Updated: 2025/08/15 16:02:30 by lsurco-t         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	parse_word(char *input, int *i, t_token **tokens)
{
	int	len;
	int	quote_start;
	int	quote_len;
	char *word_part;
	char *quote_part;
	char *combined;

	len = 0;
	while (input[*i + len] && !is_quote(input[*i + len])
		&& !is_space(input[*i + len]) && !is_special(input[*i + len]))
		len++;
		
	// Check if there's a quote immediately after the word (no space)
	if (input[*i + len] && is_quote(input[*i + len]))
	{
		// Extract the word part
		word_part = ft_strndup(&input[*i], len);
		if (!word_part)
			return (free_tokens_input(tokens, input));
			
		// Parse the quote part
		quote_start = *i + len;
		quote_len = 1;
		while (input[quote_start + quote_len] && input[quote_start + quote_len] != input[quote_start])
			quote_len++;
		if (!input[quote_start + quote_len])
		{
			free(word_part);
			return (-1);
		}
		quote_len++; // Include closing quote
		
		// Extract quote content (without outer quotes)
		quote_part = ft_strndup(&input[quote_start + 1], quote_len - 2);
		if (!quote_part)
		{
			free(word_part);
			return (free_tokens_input(tokens, input));
		}
		
		// Combine word and quote content
		combined = ft_strjoin(word_part, quote_part);
		free(word_part);
		free(quote_part);
		if (!combined)
			return (free_tokens_input(tokens, input));
			
		// Determine quote type
		t_quote_type quote_type = DOUBLE;
		if (input[quote_start] == '\'')
			quote_type = SINGLE;
		else
		{
			// Check for dollar signs in the original quoted content
			int has_dollar = 0;
			for (int j = quote_start + 1; j < quote_start + quote_len - 1; j++)
			{
				if (input[j] == '$')
				{
					has_dollar = 1;
					break;
				}
			}
			if (has_dollar)
				quote_type = DOUBLE_ENV;
		}
			
		if (add_token(tokens, quote_type, combined, ft_strlen(combined)) < 0)
		{
			free(combined);
			return (free_tokens_input(tokens, input));
		}
		free(combined);
		*i += len + quote_len;
	}
	else
	{
		if (add_token(tokens, NONE, &input[*i], len) < 0)
			return (free_tokens_input(tokens, input));
		*i += len;
	}
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
		if (input[i])  // Only parse if we haven't reached the end of input
		{
			if (handle_parsing(input, &i, tokens) < 0)
				return (-1);
		}
	}
	return (0);
}
