/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:28:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 20:25:06 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"
#include "utils.h"

int	setup_heredoc_for_pipeline(t_command *command, t_executor *executor)
{
	int	pipefd[2];

	if (!command || !command->has_heredoc || !command->heredoc_delim)
		return (FAILURE);
	if (create_heredoc_pipe(pipefd) == FAILURE)
		return (FAILURE);
	setup_signals_heredoc();
	write_heredoc_content(pipefd[1], command->heredoc_delim, executor);
	close(pipefd[1]);
	command->heredoc_fd = pipefd[0];
	return (SUCCESS);
}

int	setup_heredoc(t_command *command, t_executor *executor)
{
	int	pipefd[2];

	if (!command || !command->has_heredoc || !command->heredoc_delim)
		return (FAILURE);
	if (create_heredoc_pipe(pipefd) == FAILURE)
		return (FAILURE);
	setup_signals_heredoc();
	write_heredoc_content(pipefd[1], command->heredoc_delim, executor);
	close(pipefd[1]);
	return (setup_heredoc_input(pipefd));
}

int	create_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	setup_heredoc_input(int pipefd[2])
{
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(pipefd[0]);
		return (FAILURE);
	}
	close(pipefd[0]);
	return (SUCCESS);
}
