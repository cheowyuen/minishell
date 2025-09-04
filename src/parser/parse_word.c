/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:37:31 by cwong             #+#    #+#             */
/*   Updated: 2025/09/04 13:35:00 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	process_word_loop(char *input, t_quote *quote, char **joined,
	t_word *w)
{
	int		start;
	int		j;
	bool	has_quotes;
	char	*trimmed;
	int		starting_pos;

	starting_pos = *(w->i);
	has_quotes = false;
	while (input[*(w->i)] && !is_space(input[*(w->i)]) && !is_special(input[*(w->i)]))
	{
		if (is_quote(input[*(w->i)]))
		{
			if (*(w->i) == starting_pos)
				has_quotes = true;
			*(w->is_quoted) = true;
			if (handle_quoted_word(joined, quote, input, w->i) < 0)
				return (-1);
			if (has_quotes && (*joined)[0] != '\0')
			{
				trimmed = ft_strtrim(*joined, " ");
				if (!trimmed)
					return (-1);
				if (trimmed[0] == '\0')
				{
					*(w->temp) = ft_substr(*joined, 0, ft_strlen(*joined) - 1);
					if (!*(w->temp))
					{
						free(trimmed);
						return (-1);
					}
				}
				free(trimmed);
				has_quotes = false;
			}
		}
		else
		{
			start = *(w->i);
			if (handle_unquoted_word(joined, quote, input, w->i) < 0)
				return (-1);
			j = start;
			while (j < *(w->i))
			{
				if (input[j] == '$')
				{
					*(w->has_unquoted_vars) = true;
					break ;
				}
				j++;
			}
		}
	}
	return 0;
}


int	parse_word(char *input, int *i, t_token **tokens, t_quote *quote)
{
	char	*joined;
	bool	is_quoted;
	bool	has_unquoted_vars;
	char	*temp;
	t_word	w;

	quote->raw_value = NULL;
	joined = ft_strdup("");
	if (!joined)
		return (free_tokens_input(tokens, NULL, 1));
	is_quoted = false;
	has_unquoted_vars = false;
	temp = NULL;
	w.i = i;
	w.is_quoted = &is_quoted;
	w.has_unquoted_vars = &has_unquoted_vars;
	w.temp = &temp;
	if (process_word_loop(input, quote, &joined, &w) < 0)
	{
		free(joined);
		return free_tokens_input(tokens, NULL, 1);
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
