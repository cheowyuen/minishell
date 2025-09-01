/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/01 12:25:07 by lsurco-t         ###   ########.fr       */
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

void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("  redir type: %d, filename: %s\n",
			redir->type,
			redir->filename ? redir->filename : "(null)");
		redir = redir->next;
	}
}

void	print_commands(t_command *cmd)
{
	int i;

	while (cmd)
	{
		printf("=== Command ===\n");

		// argv
		if (cmd->argv)
		{
			printf("argv: ");
			for (i = 0; cmd->argv[i]; i++)
				printf("[%s] ", cmd->argv[i]);
			printf("\n");
		}

		printf("append: %d\n", cmd->append);
		printf("has_heredoc: %d\n", cmd->has_heredoc);
		if (cmd->heredoc_delim)
			printf("heredoc_delim: %s\n", cmd->heredoc_delim);
		printf("is_quoted: %d\n", cmd->is_quoted);
		printf("is_builtin: %d\n", cmd->is_builtin);
		printf("is_last: %d\n", cmd->is_last);
		printf("pipe_out: %d\n", cmd->pipe_out);

		// redirs
		if (cmd->redirs)
			print_redirs(cmd->redirs);

		printf("=============\n\n");
		cmd = cmd->next;
	}
}

static int	parse_and_execute(char *input, t_token **tokens,
		t_executor *executor)
{
	t_command	*commands;
	int			result;
	int			parse_result;

	commands = NULL;
	if (parse_input(input, tokens, executor) < 0)
	{
		ft_lstclear(tokens);
		return (print_syntax_error(NULL, executor, 2));
	}
	if (validate_syntax(*tokens, executor) < 0)
	{
		ft_lstclear(tokens);
		executor->last_exit_code = 2;
		return (-1);
	}
	parse_result = parse_commands(*tokens, &commands, executor);
	//print_commands(commands);
	if (parse_result == HEREDOC_LIMIT_EXCEEDED)
	{
		ft_lstclear(tokens);
		executor->last_exit_code = 2;
		return (HEREDOC_LIMIT_EXCEEDED);
	}
	if (parse_result < 0)
	{
		ft_lstclear(tokens);
		return (print_syntax_error(NULL, executor, 2));
	}
	if (*tokens)
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
		if (isatty(STDIN_FILENO))
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
		{
			if (!isatty(STDIN_FILENO))
			{
				free(input);
				return (-1);
			}
			return (free_input_failure(input));
		}
	}
	free(input);
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
			int dev_null_fd = open("/dev/null", O_WRONLY);
			FILE *dev_null_stream = fdopen(dev_null_fd, "w");
			rl_outstream = dev_null_stream;
			input = readline("");
			if (rl_outstream != stdout)
			{
				fclose(rl_outstream);
				rl_outstream = stdout;
			}
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
