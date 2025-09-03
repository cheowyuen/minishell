/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 10:23:27 by cwong             #+#    #+#             */
/*   Updated: 2025/09/03 17:46:55 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

void	ft_lstclear(t_token **lst)
{
	t_token	*temp;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		temp = *lst;
		*lst = (*lst)->next;
		free(temp->token);
		free(temp->raw_value);
		free(temp);
	}
	*lst = NULL;
}

static void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*end;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		new->next = NULL;
		return ;
	}
	end = *lst;
	while (end->next)
		end = end->next;
	end->next = new;
	new->next = NULL;
}

static t_token	*ft_lstnew(bool is_quoted, char *content, t_token_data *data)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->token = ft_strndup(content, data->len);
	if (!new_node->token)
	{
		free(new_node);
		return (NULL);
	}
	new_node->is_quoted = is_quoted;
	if (data->raw_value)
		new_node->raw_value = ft_strdup(data->raw_value);
	else
		new_node->raw_value = NULL;
	new_node->words_count = data->words_count;
	new_node->next = NULL;
	return (new_node);
}

int	add_token(t_token **tokens, bool is_quoted, char *content,
		t_token_data *data)
{
	t_token	*new_token;

	new_token = ft_lstnew(is_quoted, content, data);
	if (!new_token)
		return (-1);
	ft_lstadd_back(tokens, new_token);
	return (0);
}

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

int	add_tokens_with_splitting(t_token **tokens, bool is_quoted, char *content,
		char *raw_value)
{
	char			**words;
	int				i;
	t_token_data	data;
	bool			from_expansion;

	data.len = ft_strlen(content);
	data.raw_value = raw_value;
	// If quoted, don't split - treat as single token
	if (is_quoted)
		return (add_token(tokens, is_quoted, content, &data));
	from_expansion = (ft_strchr(content, '<') || ft_strchr(content, '>')
			|| ft_strchr(content, '|'));
	if (content[0] == '\0')
	{
		data.len = 0;
		return (add_token(tokens, is_quoted, "", &data));
	}
	// Split on spaces and tabs for unquoted content
	words = split_whitespace(content);
	if (!words)
		return (-1);
	// Add each word as a separate token
	data.words_count = 0;
	while (words[data.words_count])
		data.words_count++;
	i = 0;
	while (words[i])
	{
		if (i > 0)
			data.raw_value = NULL;
		if (add_split_token(tokens, words[i], &data, from_expansion) < 0)
		{
			free_str_array(words);
			return (-1);
		}
		i++;
	}
	data.words_count = i;
	free_str_array(words);
	return (0);
}
