/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:29 by cwong             #+#    #+#             */
/*   Updated: 2025/09/04 11:08:30 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "utils.h"

int	handle_special(t_token **tokens, t_command **current,
	int *heredoc_count, t_executor *executor)
{
	int	result;

	if (ft_strcmp((*tokens)->token, "<<") == 0)
		result = handle_heredoc(tokens, *current, heredoc_count, executor);
	else if (ft_strcmp((*tokens)->token, ">>") == 0
		|| (*tokens)->token[0] == '<' || (*tokens)->token[0] == '>')
	{
		result = set_redir(tokens, &(*current)->redirs);
		if (result < 0)
			free_tokens_commands(tokens, &executor->commands);
	}
	else
		result = 0;
	*tokens = (*tokens)->next;
	return (result);
}

static int	handle_commands(t_token **tokens, t_command *current,
	t_command **commands, t_executor *executor)
{
	int		count;
	int		heredoc_count;

	heredoc_count = 0;
	count = count_tokens(*tokens);
	current->argv = ft_calloc(count + 1, sizeof(char *));
	if (!current->argv)
		return (free_tokens_commands(tokens, commands));
	if (fill_argv_from_tokens(tokens, current, executor, &heredoc_count) < 0)
		return (free_tokens_commands(tokens, commands));
	return (0);
}

static int	process_tokens(t_token *tokens, t_command **commands,
	t_executor *executor)
{
	t_command	*current;
	int			result;

	current = *commands;
	while (tokens)
	{
		result = handle_commands(&tokens, current, commands, executor);
		if (result < 0)
			return (result);
		if (tokens && !tokens->is_quoted && ft_strcmp(tokens->token, "|") == 0)
		{
			tokens = tokens->next;
			current->pipe_out = 1;
			current->next = create_command();
			if (!current->next)
				return (free_tokens_commands(&tokens, commands));
			current = current->next;
		}
	}
	current->is_last = 1;
	return (0);
}

static int	process_collected_heredocs(t_command *commands,
		t_executor *executor)
{
	t_command	*current;
	int			result;

	current = commands;
	while (current)
	{
		if (current->has_heredoc && current->heredoc_list)
		{
			result = process_all_heredocs(current, executor);
			if (result == FAILURE)
			{
				free_commands(&executor->commands);
				executor->commands = NULL;
				return (HEREDOC_INTERRUPTED);
			}
			/* Clean up the heredoc list after processing */
			free_heredoc_list(current->heredoc_list);
			current->heredoc_list = NULL;
		}
		current = current->next;
	}
	return (SUCCESS);
}

int	parse_commands(t_token *tokens, t_command **commands, t_executor *executor)
{
	int	result;

	*commands = create_command();
	if (!*commands)
		return (free_tokens(&tokens));
	executor->commands = *commands;
	result = process_tokens(tokens, commands, executor);
	if (result < 0)
	{
		if (!executor->commands)
			*commands = NULL;
		return (result);
	}
	/* Process all collected heredocs after parsing is complete */
	result = process_collected_heredocs(*commands, executor);
	if (result < 0)
		return (result);
	return (0);
}
