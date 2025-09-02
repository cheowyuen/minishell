/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:29 by cwong             #+#    #+#             */
/*   Updated: 2025/09/02 18:30:29 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "utils.h"

static int	handle_special(t_token **tokens, t_command **current,
		int *heredoc_count, t_executor *executor)
{
	if (ft_strcmp((*tokens)->token, "<<") == 0)
	{
		(*heredoc_count)++;
		if (*heredoc_count > HEREDOC_MAX)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: maximum here-document count exceeded\n");
			free_commands(&executor->commands);
			return (HEREDOC_LIMIT_EXCEEDED);
		}
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			print_syntax_error("newline", NULL, 0);
			return (free_tokens_commands(tokens, &executor->commands));
		}
		(*current)->has_heredoc = 1;
		(*current)->is_quoted = (*tokens)->is_quoted;
		/* Add heredoc to the list instead of processing immediately */
		if (!add_heredoc_to_list(&(*current)->heredoc_list, (*tokens)->token,
				(*tokens)->is_quoted))
			return (free_tokens_commands(tokens, &executor->commands));
		/* Store delimiter for backward compatibility */
		if ((*current)->heredoc_delim)
			free((*current)->heredoc_delim);
		(*current)->heredoc_delim = process_heredoc_delimiter((*tokens)->token,
				(*tokens)->is_quoted, executor);
		if (!(*current)->heredoc_delim)
			return (free_tokens_commands(tokens, &executor->commands));
	}
	else if (ft_strcmp((*tokens)->token, ">>") == 0
		|| (*tokens)->token[0] == '<' || (*tokens)->token[0] == '>')
	{
		if (set_redir(tokens, &(*current)->redirs) < 0)
			return (free_tokens_commands(tokens, &executor->commands));
	}
	*tokens = (*tokens)->next;
	return (0);
}

static int	handle_commands(t_token **tokens, t_command *current,
		t_command **commands, t_executor *executor)
{
	int		count;
	int		argv_index;
	t_token	*tmp;
	int		heredoc_count;
	int		result;

	heredoc_count = 0;
	count = count_tokens(*tokens);
	current->argv = ft_calloc(count + 1, sizeof(char *));
	if (!current->argv)
		return (free_tokens_commands(tokens, commands));
	argv_index = 0;
	tmp = *tokens;
	while (tmp && (tmp->is_quoted || ft_strcmp(tmp->token, "|") != 0))
	{
		// printf("quoted: %d\n", tmp->is_quoted);
		if (!tmp->is_quoted && (tmp->token[0] == '<' || tmp->token[0] == '>'))
		{
			result = handle_special(&tmp, &current, &heredoc_count, executor);
			if (result < 0)
				return (result);
			continue ;
		}
		if (tmp->token && (tmp->token[0] || tmp->is_quoted))
		{
			current->argv[argv_index] = ft_strdup(tmp->token);
			if (!current->argv[argv_index])
				return (free_tokens_commands(tokens, commands));
			if (argv_index == 0 && is_builtin(tmp->token))
				current->is_builtin = 1;
			argv_index++;
		}
		tmp = tmp->next;
	}
	current->argv[argv_index] = NULL;
	*tokens = tmp;
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
