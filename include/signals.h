/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:49:46 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/18 19:59:14 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <unistd.h>
# include "minishell.h"
# include "executor.h"

extern volatile sig_atomic_t	g_signal_status;

# define SIGNAL_NONE 0
# define SIGNAL_INT 1
# define SIGNAL_QUIT 2

void							sigint_handler(int sig);
int								wait_for_child(pid_t pid);
void							setup_signals_prompt(void);
void							setup_signals_child(void);
void							setup_signals_heredoc(void);
void							setup_signals_ignore(void);

#endif