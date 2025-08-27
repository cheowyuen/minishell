/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:45:00 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/27 20:23:07 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

static int	check_redirection_syntax(t_token *current, t_token *next,
		t_executor *executor)
{
	(void)current;
	if (!next)
		return (print_syntax_error("newline", executor, 0));
	if (!next->is_quoted && is_special(next->token[0]))
		return (print_syntax_error(next->token, executor, 0));
	return (0);
}

static int	check_pipe_syntax(t_token *current, t_token *next,
		t_executor *executor)
{
	(void)current;
	if (!next)
		return (print_syntax_error("newline", executor, 0));
	if (!next->is_quoted && next->token[0] == '|')
		return (print_syntax_error("|", executor, 0));
	return (0);
}

static int	validate_token_sequence(t_token *tokens, t_executor *executor)
{
	t_token	*current;

	current = tokens;
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

int	validate_syntax(t_token *tokens, t_executor *executor)
{
	if (!tokens)
		return (0);
	if (!tokens->is_quoted && tokens->token[0] == '|')
		return (print_syntax_error("|", executor, 0));
	return (validate_token_sequence(tokens, executor));
}
