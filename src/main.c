/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/18 09:28:11 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"

static void	initial_setup(t_executor *executor, t_command *command,
				char **envp);
static int	run_shell_loop(t_executor *executor);
static void	cleanup_ending(t_executor *executor);

int	main(int argc, char **argv, char **envp)
{
	t_executor	executor;
	t_command	command;

	(void)argc;
	(void)argv;
	initial_setup(&executor, &command, envp);
	if (run_shell_loop(&executor) < 0)
		return (1);
	rl_clear_history();
	return (0);
}

static void	initial_setup(t_executor *executor, t_command *command, char **envp)
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

	tokens = NULL;
	while (1)
	{
		input = readline(CLR_GREEN PROMPT RESET);
		if (!input)
		{
			cleanup_ending(executor);
			break ;
		}
		if (g_signal_status == SIGNAL_INT)
		{
			g_signal_status = SIGNAL_NONE;
			if (input)
				free(input);
			continue ;
		}
		if (*input && ft_strlen(input) > 0 && !is_only_space_or_tab(input))
			if (parse_and_exec(input, tokens, executor) < 0)
				return (-1);
		free(input);
	}
	return (0);
}

static void	cleanup_ending(t_executor *executor)
{
	printf(CLR_RED "Exiting Minishell...\n" RESET);
	free_executor(executor);
	free_environment(executor);
}
