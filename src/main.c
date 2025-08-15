/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/08/15 08:14:05 by cwong            ###   ########.fr       */
=======
/*   Updated: 2025/08/14 11:23:16 by lsurco-t         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"
<<<<<<< HEAD
#include "executor.h"
#include <signal.h>

volatile sig_atomic_t	g_signal;

static void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		write(1, "\n", 1); 
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
=======
#include "signals.h"
>>>>>>> main

static void	initial_setup(t_executor *executor, t_command *command,
				char **envp);
static void	run_shell_loop(t_executor *executor);
static void	cleanup_ending(t_executor *executor);

int	main(int argc, char **argv, char **envp)
{
<<<<<<< HEAD
    char 	*input;
	t_token	*tokens;
	t_shell	shell;

	printf(BOLD CLR_BLUE"Welcome to Minishell!\n"RESET);
	tokens = NULL;
	//for debugging
	shell.exit_code = 0;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
    while (1)
    {
        input = readline(PROMPT);
        if (!input)
            break;
        if (*input)
		{
            add_history(input);
			if (parse_input(input, &tokens) < 0)
				return (1);
			free(input);
			if (expand_env_var(tokens, &shell) < 0)
				return (1);
			if (parse_commands(tokens) < 0)
				return (1);
			//for debugging
			t_token *tmp = tokens;
			printf("Tokens:\n");
			while (tmp)
			{
				printf("  %s\n", tmp->token);
				tmp = tmp->next;
			}
		}
		free_tokens(&tokens);
    }
    return 0;
=======
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
>>>>>>> main
}
