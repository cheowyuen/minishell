/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuen <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:20:34 by cyuen             #+#    #+#             */
/*   Updated: 2025/09/05 07:44:09 by cyuen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"

static int	handle_parse_error(t_token **tokens, t_executor *executor)
{
	ft_lstclear(tokens);
	return (print_syntax_error(NULL, executor, 2));
}

static int	handle_syntax_error(t_token **tokens, t_executor *executor,
	int code)
{
	ft_lstclear(tokens);
	executor->last_exit_code = 2;
	return (code);
}

static int	handle_heredoc_interrupt(t_token **tokens)
{
	ft_lstclear(tokens);
	return (HEREDOC_INTERRUPTED);
}

int	parse_and_execute(char *input, t_token **tokens, t_executor *executor)
{
	t_command	*commands;
	int			result;
	int			parse_result;

	commands = NULL;
	if (parse_input(input, tokens, executor) < 0)
		return (handle_parse_error(tokens, executor));
	if (validate_syntax(*tokens, executor) < 0)
		return (handle_syntax_error(tokens, executor, -1));
	parse_result = parse_commands(*tokens, &commands, executor);
	if (parse_result == HEREDOC_LIMIT_EXCEEDED)
		return (handle_syntax_error(tokens, executor, HEREDOC_LIMIT_EXCEEDED));
	if (parse_result == HEREDOC_INTERRUPTED)
		return (handle_heredoc_interrupt(tokens));
	if (parse_result < 0)
		return (handle_parse_error(tokens, executor));
	ft_lstclear(tokens);
	if (commands)
	{
		result = exec_cmd(executor, commands);
		executor->last_exit_code = result;
		free_commands(&commands);
		executor->commands = NULL;
	}
	return (0);
}
