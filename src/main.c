/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:23:16 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"

static void	initial_setup(t_executor *executor, t_command *command,
				char **envp);
static void	run_shell_loop(t_executor *executor);
static void	cleanup_ending(t_executor *executor);

int	main(int argc, char **argv, char **envp)
{
	t_executor	executor;
	t_command	command;

	(void)argc;
	(void)argv;
	initial_setup(&executor, &command, envp);
	run_shell_loop(&executor);
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

static void	run_shell_loop(t_executor *executor)
{
	char	*input;

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
			add_history(input);
		free(input);
	}
}

static void	cleanup_ending(t_executor *executor)
{
	printf(CLR_RED "Exiting Minishell...\n" RESET);
	free_executor(executor);
	free_environment(executor);
}
