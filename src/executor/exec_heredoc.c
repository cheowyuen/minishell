/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:28:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/01 17:06:49 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"
#include "utils.h"

int	setup_heredoc_for_pipeline(t_command *command)
{
	int	pipefd[2];

	if (!command || !command->has_heredoc)
		return (FAILURE);
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (FAILURE);
	}
	setup_signals_heredoc();
	if (command->heredoc_content)
	{
		if (write(pipefd[1], command->heredoc_content, 
			ft_strlen(command->heredoc_content)) == -1)
			perror("write");
	}
	close(pipefd[1]);
	command->heredoc_fd = pipefd[0];
	return (SUCCESS);
}

int	setup_heredoc(t_command *command)
{
	int	pipefd[2];

	if (!command || !command->has_heredoc)
		return (FAILURE);
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (FAILURE);
	}
	setup_signals_heredoc();
	if (command->heredoc_content)
	{
		if (write(pipefd[1], command->heredoc_content, 
			ft_strlen(command->heredoc_content)) == -1)
			perror("write");
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(pipefd[0]);
		return (FAILURE);
	}
	close(pipefd[0]);
	return (SUCCESS);
}
