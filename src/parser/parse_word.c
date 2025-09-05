/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuen <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:37:31 by cwong             #+#    #+#             */
/*   Updated: 2025/09/05 07:44:01 by cyuen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	handle_unquoted(t_word *w, char **joined, t_quote *quote,
	char *input)
{
	int	start;
	int	j;

	start = *(w->i);
	if (handle_unquoted_word(joined, quote, input, w->i) < 0)
		return (-1);
	j = start;
	while (j < *(w->i))
	{
		if (input[j] == '$')
		{
			w->has_unquoted_vars = true;
			break ;
		}
		j++;
	}
	return (0);
}

static int	process_word_loop(char *input, t_quote *quote, char **joined,
	t_word *w)
{
	w->starting_pos = *(w->i);
	while (input[*(w->i)] && !is_space(input[*(w->i)])
		&& !is_special(input[*(w->i)]))
	{
		if (is_quote(input[*(w->i)]))
		{
			if (handle_quoted(w, joined, quote, input) < 0)
				return (-1);
		}
		else
		{
			if (handle_unquoted(w, joined, quote, input) < 0)
				return (-1);
		}
	}
	return (0);
}

static int	cleanup_error(char *joined, t_quote *quote, t_token **tokens)
{
	free(joined);
	free(quote->raw_value);
	return (free_tokens_input(tokens, NULL, 1));
}

static int	handle_temp_token(t_token **tokens, t_word *w, t_quote *quote
	, char *joined)
{
	if (!w->temp)
		return (0);
	if (w->has_unquoted_vars)
	{
		if (add_tokens_with_splitting(tokens, w->is_quoted, w->temp,
				quote->raw_value) < 0)
		{
			free(w->temp);
			return (-1);
		}
	}
	free(w->temp);
	return (0);
}

int	parse_word(char *input, int *i, t_token **tokens, t_quote *quote)
{
	char	*joined;
	t_word	w;

	quote->raw_value = NULL;
	joined = ft_strdup("");
	if (!joined)
		return (free_tokens_input(tokens, NULL, 1));
	w.i = i;
	w.is_quoted = false;
	w.has_unquoted_vars = false;
	w.temp = NULL;
	if (process_word_loop(input, quote, &joined, &w) < 0)
		return (cleanup_error(joined, quote, tokens));
	if (handle_temp_token(tokens, &w, quote, joined) < 0)
		return (cleanup_error(joined, quote, tokens));
	if (add_tokens_with_splitting(tokens, w.is_quoted && !w.has_unquoted_vars,
			joined, quote->raw_value) < 0)
		return (cleanup_error(joined, quote, tokens));
	free(joined);
	free(quote->raw_value);
	return (0);
}
