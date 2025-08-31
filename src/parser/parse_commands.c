/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuen <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:29 by cwong             #+#    #+#             */
/*   Updated: 2025/08/31 11:37:04 by cyuen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "utils.h"

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
			free_commands(commands);
			return (HEREDOC_LIMIT_EXCEEDED);
		}
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			print_syntax_error("newline", NULL, 0);
			return (free_tokens_commands(tokens, commands));
		}
		(*current)->has_heredoc = 1;
		(*current)->is_quoted = (*tokens)->is_quoted;
		if ((*current)->heredoc_delim)
			free((*current)->heredoc_delim);
		(*current)->heredoc_delim = ft_strdup((*tokens)->token);
		if (!(*current)->heredoc_delim)
			return (free_tokens_commands(tokens, commands));
	}
	// else if (ft_strcmp((*tokens)->token, ">>") == 0)
	// {
	// 	if (set_outfile(tokens, 1, &(*current)->outfiles) < 0)
	// 		return (free_tokens_commands(tokens, commands));
	// }
	else if (ft_strcmp((*tokens)->token, ">>") == 0
		|| (*tokens)->token[0] == '<'
		|| (*tokens)->token[0] == '>')
	{
		// if (set_infile(tokens, &(*current)->infiles) < 0)
		if (set_redir(tokens, &(*current)->redirs) < 0)
			return (free_tokens_commands(tokens, commands));
	}
	// else if ((*tokens)->token[0] == '>')
	// {
	// 	if (set_outfile(tokens, 0, &(*current)->outfiles) < 0)
	// 		return (free_tokens_commands(tokens, commands));
	// }
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
		if (!tmp->is_quoted && (tmp->token[0] == '<' || tmp->token[0] == '>'))
        {
			result = handle_special(&tmp, &current, commands, &heredoc_count);
			if (result < 0)
				return (result);  // Propagate special codes like HEREDOC_LIMIT_EXCEEDED
            continue ; 
        }
		if (tmp->token && (tmp->token[0] || tmp->is_quoted)) // Skip empty tokens only if unquoted (from environment variables that expand to empty)
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
	int 		result;

	current = *commands;
	while (tokens)
	{
		result = handle_commands(&tokens, current, commands);
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

int	parse_commands(t_token *tokens, t_command **commands)
{
	int result;

	*commands = create_command();
	if (!*commands)
		return (free_tokens(&tokens));
	result = process_tokens(tokens, commands);
	if (result < 0)
		return (result);
	return (0);
}
