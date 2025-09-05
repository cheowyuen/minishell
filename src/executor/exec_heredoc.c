/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:28:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/05 10:17:25 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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
