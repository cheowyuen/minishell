/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 12:58:41 by cwong             #+#    #+#             */
/*   Updated: 2025/09/06 12:58:46 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->filename);
		free(redirs);
		redirs = next;
	}
}

static void	free_single_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv);
	}
	free_redirs(cmd->redirs);
	free(cmd->heredoc_delim);
	free(cmd->heredoc_content);
	if (cmd->heredoc_fd >= 0)
		close(cmd->heredoc_fd);
	if (cmd->heredoc_list)
		free_heredoc_list(cmd->heredoc_list);
	free(cmd);
}

void	free_commands(t_command **commands)
{
	t_command	*temp;

	if (!commands || !*commands)
		return ;
	while (*commands)
	{
		temp = *commands;
		*commands = (*commands)->next;
		free_single_command(temp);
	}
	*commands = NULL;
}

int	free_tokens_commands(t_token **tokens, t_command **commands)
{
	free_tokens(tokens);
	free_commands(commands);
	return (-1);
}

int	free_tokens_input(t_token **tokens, char *input, int flag)
{
	if (flag)
		free_tokens(tokens);
	else if (!flag)
	{
		if (input)
		{
			free_tokens(tokens);
			free(input);
		}
	}
	return (-1);
}
