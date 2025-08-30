/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 10:23:27 by cwong             #+#    #+#             */
/*   Updated: 2025/08/30 20:14:29 by lsurco-t         ###   ########.fr       */
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

static t_token	*ft_lstnew(bool is_quoted, char *content, int len)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->token = ft_strndup(content, len);
	if (!new_node->token)
	{
		free(new_node);
		return (NULL);
	}
	new_node->is_quoted = is_quoted;
	new_node->next = NULL;
	return (new_node);
}

int	add_token(t_token **tokens, bool is_quoted, char *content, int len)
{
	t_token	*new_token;

	new_token = ft_lstnew(is_quoted, content, len);
	if (!new_token)
		return (-1);
	ft_lstadd_back(tokens, new_token);
	return (0);
}

static int	add_split_token(t_token **tokens, char *word, bool from_expansion)
{
	t_token	*new_token;

	new_token = ft_lstnew(from_expansion, word, ft_strlen(word));
	if (!new_token)
		return (-1);
	ft_lstadd_back(tokens, new_token);
	return (0);
}

int	add_tokens_with_splitting(t_token **tokens, bool is_quoted, char *content)
{
	char	**words;
	int		i;
	bool	from_expansion;

	if (is_quoted)
		return (add_token(tokens, is_quoted, content, ft_strlen(content)));
	from_expansion = (ft_strchr(content, '<') != NULL || ft_strchr(content,
				'>') != NULL || ft_strchr(content, '|') != NULL);
	words = ft_split(content, ' ');
	if (!words)
		return (-1);
	i = 0;
	while (words[i])
	{
		if (add_split_token(tokens, words[i], from_expansion) < 0)
		{
			free_str_array(words);
			return (-1);
		}
		i++;
	}
	free_str_array(words);
	return (0);
}
