/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:29 by cwong             #+#    #+#             */
/*   Updated: 2025/08/26 12:03:21 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

static int	handle_special(t_token **tokens, t_command **current,
		t_command **commands, int *heredoc_count)
{
	if (ft_strcmp((*tokens)->token, "<<") == 0)
	{
		(*heredoc_count)++;
		if (*heredoc_count > HEREDOC_MAX)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: maximum here-document count exceeded\n");
			return (free_tokens_commands(tokens, commands));
		}
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token `newline'\n");
			return (free_tokens_commands(tokens, commands));
		}
		(*current)->has_heredoc = 1;
		(*current)->is_quoted = (*tokens)->is_quoted;
		(*current)->heredoc_delim = ft_strdup((*tokens)->token);
		if (!(*current)->heredoc_delim)
			return (free_tokens_commands(tokens, commands));
	}
	else if (ft_strcmp((*tokens)->token, ">>") == 0)
	{
		if (set_outfile(tokens, 1, &(*current)->outfiles) < 0)
			return (free_tokens_commands(tokens, commands));
	}
	else if ((*tokens)->token[0] == '<')
	{
		if (set_infile(tokens, *current) < 0)
			return (free_tokens_commands(tokens, commands));
	}
	else if ((*tokens)->token[0] == '>')
	{
		if (set_outfile(tokens, 0, &(*current)->outfiles) < 0)
			return (free_tokens_commands(tokens, commands));
	}
	*tokens = (*tokens)->next;
	return (0);
}

static int	handle_commands(t_token **tokens, t_command *current,
		t_command **commands)
{
	int		count;
	int		argv_index;
	t_token	*tmp;
	int		heredoc_count;

	heredoc_count = 0;
	count = count_tokens(*tokens);
	current->argv = ft_calloc(count + 1, sizeof(char *));
	if (!current->argv)
		return (free_tokens_commands(tokens, commands));
	argv_index = 0;
	tmp = *tokens;
	while (tmp && (tmp->is_quoted || ft_strcmp(tmp->token, "|") != 0))
	{
		if (!tmp->is_quoted && (tmp->token[0] == '<' || tmp->token[0] == '>'))
        {
			if (handle_special(&tmp, &current, commands, &heredoc_count) < 0)
				return (-1);
            continue ; 
        }
		if (tmp->token && tmp->token[0]) // Skip empty tokens (from environment variables that expand to empty)
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

static int	process_tokens(t_token *tokens, t_command **commands)
{
	t_command	*current;

	current = *commands;
	while (tokens)
	{
		if (handle_commands(&tokens, current, commands) < 0)
			return (-1);
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

int	parse_commands(t_token *tokens, t_command **commands)
{
	*commands = create_command();
	if (!*commands)
		return (free_tokens(&tokens));
	if (process_tokens(tokens, commands) < 0)
		return (-1);
	return (0);
}
