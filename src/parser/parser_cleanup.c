/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:30:25 by cwong             #+#    #+#             */
/*   Updated: 2025/08/15 10:13:10 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	free_tokens(t_token **tokens)
{
	if (tokens && *tokens)
		ft_lstclear(tokens);
	return (-1);
}

int	free_tokens_commands(t_token **tokens, t_command **commands)
{
	t_command	*temp;
	int			i;

	free_tokens(tokens);
	if (!commands || !*commands)
		return (-1);
	while (*commands)
	{
		temp = *commands;
		*commands = (*commands)->next;
		i = 0;
		while (temp->argv[i])
		{
			free(temp->argv[i]);
			i++;
		}
		free(temp->argv);
		free(temp);
	}
	*commands = NULL;
	return (-1);
}

int	free_tokens_input(t_token **tokens, char *input)
{
	free_tokens(tokens);
	if (input)
		free(input);
	return (-1);
}
