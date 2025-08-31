/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 10:23:27 by cwong             #+#    #+#             */
/*   Updated: 2025/08/31 18:45:29 by cwong            ###   ########.fr       */
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
	new_node->raw_value = data->raw_value;
	new_node->next = NULL;
	return (new_node);
}

int	add_token(t_token **tokens, bool is_quoted, char *content, t_token_data *data)
{
	t_token			*new_token;

	new_token = ft_lstnew(is_quoted, content, data);
	if (!new_token)
		return (-1);
	ft_lstadd_back(tokens, new_token);
	return (0);
}

static int	add_split_token(t_token **tokens, char *word, char *raw_value, bool from_expansion)
{
	t_token			*new_token;
	t_token_data	data;

	data.len = ft_strlen(word);
	data.raw_value = raw_value;
	new_token = ft_lstnew(from_expansion, word, &data);
	if (!new_token)
		return (-1);
	ft_lstadd_back(tokens, new_token);
	return (0);
}

int	add_tokens_with_splitting(t_token **tokens, bool is_quoted, char *content, char *raw_value)
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
	from_expansion = (ft_strchr(content, '<') || ft_strchr(content, '>') || ft_strchr(content, '|'));
	//printf("from exp: %d\n", from_expansion);
	if (content[0] == '\0')
	{
		data.len = 0;
		return (add_token(tokens, is_quoted, "", &data));
	}
	if (!data.raw_value || ft_strchr(content, '<') || ft_strchr(content, '>') || ft_strchr(content, '|'))
	{
		// Split on spaces for unquoted content
		words = ft_split(content, ' ');
		if (!words)
			return (-1);
		// Add each word as a separate token
		i = 0;
		while (words[i])
		{
			//printf("loop from exp: %d\nword: %s\n", from_expansion, words[i]);
			if (add_split_token(tokens, words[i], data.raw_value, from_expansion) < 0)
			{
				free_str_array(words);
				return (-1);
			}
			i++;
		}
		free_str_array(words);
		return (0);
	}
	return (add_token(tokens, is_quoted, content, &data));
}
