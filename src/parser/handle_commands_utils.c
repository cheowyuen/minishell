/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_commands_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:36:41 by cwong             #+#    #+#             */
/*   Updated: 2025/09/04 11:15:37 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "utils.h"

static int	check_heredoc_limit(int *heredoc_count, t_executor *executor)
{
	(*heredoc_count)++;
	if (*heredoc_count > HEREDOC_MAX)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: maximum here-document count exceeded\n");
		free_commands(&executor->commands);
		return (HEREDOC_LIMIT_EXCEEDED);
	}
	return (0);
}

int	handle_heredoc(t_token **tokens, t_command *current,
		int *heredoc_count, t_executor *executor)
{
	char	*delimiter_value;

	if (check_heredoc_limit(heredoc_count, executor) < 0)
		return (HEREDOC_LIMIT_EXCEEDED);
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		print_syntax_error("newline", NULL, 0);
		return (free_tokens_commands(tokens, &executor->commands));
	}
	current->has_heredoc = 1;
	current->is_quoted = (*tokens)->is_quoted;
	if ((*tokens)->raw_value)
		delimiter_value = (*tokens)->raw_value;
	else
		delimiter_value = (*tokens)->token;
	if (!add_heredoc_to_list(&current->heredoc_list,
			delimiter_value, (*tokens)->is_quoted))
		return (free_tokens_commands(tokens, &executor->commands));
	if (current->heredoc_delim)
		free(current->heredoc_delim);
	current->heredoc_delim = process_heredoc_delimiter(delimiter_value);
	if (!current->heredoc_delim)
		return (free_tokens_commands(tokens, &executor->commands));
	return (0);
}

static int	add_to_argv(t_command *current, t_token *tmp, int *argv_index)
{
	current->argv[*argv_index] = ft_strdup(tmp->token);
	if (!current->argv[*argv_index])
		return (-1);
	if (*argv_index == 0 && is_builtin(tmp->token))
		current->is_builtin = 1;
	(*argv_index)++;
	return (0);
}

int	fill_argv_from_tokens(t_token **tokens, t_command *current,
		t_executor *executor, int *heredoc_count)
{
	int		argv_index;
	t_token	*tmp;
	int		result;

	argv_index = 0;
	tmp = *tokens;
	while (tmp && (tmp->is_quoted || ft_strcmp(tmp->token, "|") != 0))
	{
		if (!tmp->is_quoted && (tmp->token[0] == '<' || tmp->token[0] == '>'))
		{
			result = handle_special(&tmp, &current, heredoc_count, executor);
			if (result < 0)
				return (result);
			continue ;
		}
		if (tmp->token && (tmp->token[0] || tmp->is_quoted))
		{
			if (add_to_argv(current, tmp, &argv_index) < 0)
				return (-1);
		}
		tmp = tmp->next;
	}
	current->argv[argv_index] = NULL;
	*tokens = tmp;
	return (0);
}
