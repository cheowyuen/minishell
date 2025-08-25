/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/25 17:45:04 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"

static int	run_shell_loop(t_executor *executor);

int	main(int argc, char **argv, char **envp)
{
	t_executor	executor;
	t_command	command;
	int			exit_code;

	(void)argc;
	(void)argv;
	setup_signals_prompt();
	ft_bzero(&executor, sizeof(t_executor));
	ft_bzero(&command, sizeof(t_command));
	executor.logical_pwd = getcwd(NULL, 0);
	if (!executor.logical_pwd)
		executor.logical_pwd = ft_strdup("/");
	add_envp_to_executor(&executor, envp);
	if (isatty(STDIN_FILENO))
		printf(BOLD CLR_BLUE "Welcome to Minishell!\n" RESET);
	run_shell_loop(&executor);
	exit_code = executor.last_exit_code;
	free_executor(&executor);
	rl_clear_history();
	return (exit_code);
}

static int	parse_and_execute(char *input, t_token **tokens,
		t_executor *executor)
{
	t_command	*commands;
	int			result;

	commands = NULL;
	if (parse_input(input, tokens, executor) < 0
		|| parse_commands(*tokens, &commands) < 0)
	{
		ft_dprintf(STDERR_FILENO,"minishell: unsupported syntax\n");
		executor->last_exit_code = 2; //exit code 2 for syntax errors
		return (-1);
	}
	if (*tokens) // Only free if tokens wasn't already freed during parsing
		ft_lstclear(tokens);
	if (commands)
	{
		result = exec_cmd(executor, commands);
		executor->last_exit_code = result;
		free_commands(&commands);
	}
	return (0);
}

static int	handle_input_processing(char *input, t_token *tokens,
		t_executor *executor)
{
	if (!input)
	{
		return (-1);
	}
	if (g_signal_status == SIGNAL_INT)
	{
		executor->last_exit_code = 130;
		g_signal_status = SIGNAL_NONE;
		if (!input || !*input || is_only_space_or_tab(input))
			return (free_input_failure(input));
	}
	if (*input && ft_strlen(input) > 0 && !is_only_space_or_tab(input))
	{
		add_history(input);
		if (parse_and_execute(input, &tokens, executor) < 0)
			return (free_input_failure(input));
	}
	free(input);
	if (!isatty(STDIN_FILENO))
		return (-1);
	return (0);
}

static int	run_shell_loop(t_executor *executor)
{
	char	*input;
	t_token	*tokens;
	int		result;

	tokens = NULL;
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			input = readline("\001" CLR_GREEN "\002" PROMPT "\001" RESET "\002");
		}
		else
			input = readline(PROMPT);
		result = handle_input_processing(input, tokens, executor);
		if (result == -1)
			break ;
		if (result == 1)
			continue ;
	}
	return (0);
}
