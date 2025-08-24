/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 10:23:27 by cwong             #+#    #+#             */
/*   Updated: 2025/08/24 20:41:21 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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
