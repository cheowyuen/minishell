/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 09:55:43 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
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

int main(void)
{
    char 	*input;
	t_token	*tokens;

	printf(BOLD CLR_BLUE"Welcome to Minishell!\n"RESET);
	tokens = NULL;
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
			if (expand_env_var(tokens) < 0)
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
}
