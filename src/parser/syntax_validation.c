/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuen <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:45:00 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/27 18:42:35 by cyuen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

static int	check_redirection_syntax(t_token *current, t_token *next,
		t_executor *executor)
{
	(void)current; // Unused parameter
	if (!next)
		return (print_syntax_error("newline", executor, 0));
	// Check if next token is also a special character (and not quoted)
	if (!next->is_quoted && is_special(next->token[0]))
		return (print_syntax_error(next->token, executor, 0));
	return (0);
}

static int	check_pipe_syntax(t_token *current, t_token *next,
		t_executor *executor)
{
	(void)current; // Unused parameter
	if (!next)
		return (print_syntax_error("newline", executor, 0));
	// Check if next token is also a pipe or other special character (and not quoted)
	if (!next->is_quoted && next->token[0] == '|')
        return (print_syntax_error("|", executor, 0));
	return (0);
}

int	validate_syntax(t_token *tokens, t_executor *executor)
{
	t_token	*current;

	if (!tokens)
		return (0);
	current = tokens;
	// Check if first token is a pipe (and not quoted)
	if (!current->is_quoted && current->token[0] == '|')
		return (print_syntax_error("|", executor, 0));
	while (current)
	{
		if (!current->is_quoted && is_special(current->token[0]))
		{
			if (current->token[0] == '|')
			{
				if (check_pipe_syntax(current, current->next, executor) < 0)
					return (-1);
			}
			else if (current->token[0] == '<' || current->token[0] == '>')
			{
				if (check_redirection_syntax(current, current->next,
						executor) < 0)
					return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}
