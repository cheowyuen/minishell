/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 11:31:06 by cwong             #+#    #+#             */
/*   Updated: 2025/09/04 11:46:32 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

static int	add_split_token(t_token **tokens, char *word, t_token_data *data,
		bool from_expansion)
{
	t_token	*new_token;

	data->len = ft_strlen(word);
	new_token = ft_lstnew(from_expansion, word, data);
	if (!new_token)
		return (-1);
	ft_lstadd_back(tokens, new_token);
	return (0);
}

static int	add_words_as_tokens(t_token **tokens, char **words,
		t_token_data *data, bool from_expansion)
{
	int	i;

	data->words_count = 0;
	while (words[data->words_count])
		data->words_count++;
	i = 0;
	while (words[i])
	{
		if (i > 0)
			data->raw_value = NULL;
		if (add_split_token(tokens, words[i], data, from_expansion) < 0)
			return (-1);
		i++;
	}
	data->words_count = i;
	return (0);
}

int	add_tokens_with_splitting(t_token **tokens, bool is_quoted,
		char *content, char *raw_value)
{
	char			**words;
	t_token_data	data;
	bool			from_expansion;

	data.len = ft_strlen(content);
	data.raw_value = raw_value;
	if (is_quoted)
		return (add_token(tokens, is_quoted, content, &data));
	from_expansion = (ft_strchr(content, '<') || ft_strchr(content, '>')
		|| ft_strchr(content, '|'));
	if (content[0] == '\0')
	{
		data.len = 0;
		return (add_token(tokens, is_quoted, "", &data));
	}
	words = split_whitespace(content);
	if (!words)
		return (-1);
	if (add_words_as_tokens(tokens, words, &data, from_expansion) < 0)
	{
		free_str_array(words);
		return (-1);
	}
	free_str_array(words);
	return (0);
}
