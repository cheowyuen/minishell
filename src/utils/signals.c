/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:52:49 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/06 12:59:21 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

volatile sig_atomic_t	g_signal_status = SIGNAL_NONE;

void	setup_signals_prompt(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_signals_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	check_signal_interrupt(void)
{
	if (g_signal_status == SIGNAL_INT)
		return (1);
	return (0);
}

void	setup_heredoc_signals(bool is_interactive)
{
	if (is_interactive)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		g_signal_status = SIGNAL_NONE;
		rl_event_hook = check_signal_interrupt;
	}
}
