/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/29 20:13:33 by lsurco-t         ###   ########.fr       */
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
	int			parse_result;

	commands = NULL;
	if (parse_input(input, tokens, executor) < 0)
		return (print_syntax_error(NULL, executor, 2));
	if (validate_syntax(*tokens, executor) < 0)
	{
		executor->last_exit_code = 2; //exit code 2 for syntax errors
		return (-1);
	}
	parse_result = parse_commands(*tokens, &commands);
	if (parse_result == HEREDOC_LIMIT_EXCEEDED)
	{
		ft_lstclear(tokens); // Free the tokens since they weren't freed in parse_commands
		executor->last_exit_code = 2; // Exit code 2 for heredoc limit exceeded
		return (HEREDOC_LIMIT_EXCEEDED);
	}
	if (parse_result < 0)
	{
		ft_lstclear(tokens); // Free the tokens since they weren't freed in parse_commands
		return (print_syntax_error(NULL, executor, 2));
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
	int result;

	if (!input)
	{
		printf("exit\n");
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
		result = parse_and_execute(input, &tokens, executor);
		if (result == HEREDOC_LIMIT_EXCEEDED)
		{
			free_input_failure(input);
			return (-1);
		}
		else if (result < 0)
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
		{
			char *line;
			line = get_next_line(fileno(stdin));
			input = ft_strtrim(line, "\n");
			free(line);
		}
		// else
		// 	input = readline(PROMPT);
		result = handle_input_processing(input, tokens, executor);
		if (result == -1)
			break ;
		if (result == 1)
			continue ;
	}
	return (0);
}
