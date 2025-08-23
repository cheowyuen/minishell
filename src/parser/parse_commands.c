/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:29 by cwong             #+#    #+#             */
/*   Updated: 2025/08/23 18:35:28 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

static int	handle_commands(t_token **tokens, t_command *current,
		t_command **commands)
{
	int	count;
	int	i;

	count = count_tokens(*tokens);
	current->argv = ft_calloc(count + 1, sizeof(char *));
	if (!current->argv)
		return (free_tokens_commands(tokens, commands));
	i = 0;
	while (i < count)
	{
		current->argv[i] = ft_strdup((*tokens)->token);
		if (!current->argv[i])
			return (free_tokens_commands(tokens, commands));
		if (i == 0 && is_builtin((*tokens)->token))
			current->is_builtin = 1;
		*tokens = (*tokens)->next;
		i++;
	}
	current->argv[i] = NULL;
	return (0);
}

static int	handle_special(t_token **tokens, t_command **current,
		t_command **commands, int *heredoc_count)
{
	t_command	*new_command;

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
		(*current)->heredoc_delim = ft_strdup((*tokens)->token);
		if (!(*current)->heredoc_delim)
			return (free_tokens_commands(tokens, commands));
	}
	else if (ft_strcmp((*tokens)->token, ">>") == 0)
	{
		if (set_outfile(tokens, *current, 1) < 0)
			return (free_tokens_commands(tokens, commands));
	}
	else if ((*tokens)->token[0] == '<')
	{
		if (set_infile(tokens, *current) < 0)
			return (free_tokens_commands(tokens, commands));
	}
	else if ((*tokens)->token[0] == '>')
	{
		if (set_outfile(tokens, *current, 0) < 0)
			return (free_tokens_commands(tokens, commands));
	}
	else
	{
		(*current)->pipe_out = 1;
		new_command = create_command();
		if (!new_command)
			return (free_tokens_commands(tokens, commands));
		(*current)->next = new_command;
		*current = (*current)->next;
	}
	*tokens = (*tokens)->next;
	return (0);
}

static int	process_tokens(t_token *tokens, t_command **commands)
{
	t_command	*current;
	int			heredoc_count;

	current = *commands;
	heredoc_count = 0;
	while (tokens)
	{
		if (is_special(tokens->token[0]))
		{
			if (handle_special(&tokens, &current, commands, &heredoc_count) < 0)
				return (-1);
		}
		else
		{
			if (handle_commands(&tokens, current, commands) < 0)
				return (-1);
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
