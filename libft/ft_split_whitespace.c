/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespace.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:43:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/03 18:13:48 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words_whitespace(const char *s)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (!(*s == ' ' || (*s >= '\t' && *s <= '\r')))
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*extract_word(const char *s, int start, int end)
{
	char	*word;
	int		i;

	word = malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (start < end)
	{
		word[i] = s[start];
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

static int	skip_to_next_word(const char *s, int *i, int *start)
{
	while (s[*i] && (s[*i] == ' ' || (s[*i] >= '\t' && s[*i] <= '\r')))
		(*i)++;
	*start = *i;
	while (s[*i] && !(s[*i] == ' ' || (s[*i] >= '\t' && s[*i] <= '\r')))
		(*i)++;
	return (*start < *i);
}

static int	process_words(char **result, const char *s, int word_count)
{
	int	i;
	int	start;
	int	word_index;

	i = 0;
	word_index = 0;
	while (s[i] && word_index < word_count)
	{
		if (skip_to_next_word(s, &i, &start))
		{
			result[word_index] = extract_word(s, start, i);
			if (!result[word_index])
			{
				while (word_index > 0)
					free(result[--word_index]);
				return (-1);
			}
			word_index++;
		}
	}
	result[word_index] = NULL;
	return (0);
}

char	**split_whitespace(const char *s)
{
	char	**result;
	int		word_count;

	if (!s)
		return (NULL);
	word_count = count_words_whitespace(s);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (process_words(result, s, word_count) == -1)
	{
		free(result);
		return (NULL);
	}
	return (result);
}
