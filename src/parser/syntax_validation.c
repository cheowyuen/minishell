/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuen <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:45:00 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/31 11:05:52 by cyuen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

static int	check_redirection_syntax(t_token *current, t_token *next,
		t_executor *executor)
{
	char	**words;
	int		count;
	
	(void)current;
	if (!next)
		return (print_syntax_error("newline", executor, 0));
	if (!next->is_quoted && is_special(next->token[0]))
		return (print_syntax_error(next->token, executor, 0));
	if (next->raw_value)
	{
		words = ft_split(next->token, ' ');
		if (!words)
			return (-1);
		count = 0;
		while (words[count])
			count++;
		free_str_array(words);
		if (next->token[0] == '\0' || count > 1)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: %s: ambiguous redirect\n", next->raw_value);
			return (-1);
		}
	}
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
