/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:49:46 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 15:52:07 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "executor.h"
# include "minishell.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <unistd.h>

// Global variable to track signal status
extern volatile sig_atomic_t	g_signal_status;

// Macro definitions for signal statuses
# define SIGNAL_NONE 0
# define SIGNAL_INT 1
# define SIGNAL_QUIT 2

// Signal handling functions
void							sigint_handler(int sig);
void							setup_signals_prompt(void);
void							setup_signals_child(void);
void							setup_signals_heredoc(void);
void							setup_signals_ignore(void);
int								wait_all_children(t_executor *executor,
									int *last_status);

#endif