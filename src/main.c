/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/19 11:50:03 by cwong            ###   ########.fr       */
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
	add_envp_to_executor(&executor, envp);
	if (isatty(STDIN_FILENO))
		printf(BOLD CLR_BLUE "Welcome to Minishell!\n" RESET);
	run_shell_loop(&executor);
	exit_code = executor.last_exit_code;
	if (executor.env_list)
		free_environment(&executor);
	free_executor(&executor);
	rl_clear_history();
	return (exit_code);
}

static int	parse_and_execute(char *input, t_token *tokens,
		t_executor *executor)
{
	t_shell		shell;
	t_command	*commands;
	int			result;

	commands = NULL;
	shell.exit_code = executor->last_exit_code;
	if (parse_input(input, &tokens) < 0)
		return (-1);
	if (expand_env_var(tokens, &shell, executor) < 0)
		return (-1);
	if (parse_commands(tokens, &commands) < 0)
		return (-1);
	ft_lstclear(&tokens);
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
	setup_signals_prompt();
	ft_bzero(executor, sizeof(t_executor));
	ft_bzero(command, sizeof(t_command));
	add_envp_to_executor(executor, envp);
	printf(BOLD CLR_BLUE "Welcome to Minishell!\n" RESET);
}

static int	parse_and_exec(char *input, t_token *tokens, t_executor *executor)
{
	t_shell		shell;
	t_command	*commands;

	commands = NULL;
	add_history(input);
	if (parse_input(input, &tokens) < 0)
		return (-1);
	free(input);
	if (expand_env_var(tokens, &shell, executor) < 0)
		return (-1);
	if (parse_commands(tokens, &commands) < 0)
		return (-1);
	executor->commands = commands;
	if (executor->commands)
		exec_cmd(executor, executor->commands);
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
			input = readline(CLR_GREEN PROMPT RESET);
		}
		else
			input = readline("");
		result = handle_input_processing(input, tokens, executor);
		if (result == -1)
			break ;
		if (result == 1)
			continue ;
		}
		if (*input && ft_strlen(input) > 0 && !is_only_space_or_tab(input))
			if (parse_and_exec(input, tokens, executor) < 0)
				return (-1);
		free(input);
	}
	return (0);
}
