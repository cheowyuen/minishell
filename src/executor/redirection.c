/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/05 10:23:50 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	setup_stdin_redirection(int fd)
{
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	process_redir_input(t_redir *redir, bool is_last_input)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->filename);
		return (FAILURE);
	}
	if (is_last_input && setup_stdin_redirection(fd) == FAILURE)
		return (FAILURE);
	close(fd);
	return (SUCCESS);
}

static int	handle_output_redirection(t_redir *redir)
{
	int	fd;

	fd = open_redir_file(redir);
	if (fd < 0)
		return (FAILURE);
	return (redirect_stdout(fd, redir));
}

static int	process_redirection_list(t_command *command, t_redir *redirs)
{
	while (redirs)
	{
		if (redirs->type == REDIR_IN && (!command->has_heredoc
				|| command->heredoc_fd < 0))
		{
			if (process_redir_input(redirs, !has_next(redirs,
						REDIR_IN)) == FAILURE)
				return (FAILURE);
		}
		else if (redirs->type == REDIR_OUT || redirs->type == REDIR_APPEND)
		{
			if (handle_output_redirection(redirs) == FAILURE)
				return (FAILURE);
		}
		redirs = redirs->next;
	}
	return (SUCCESS);
}

int	setup_redirection(t_command *command)
{
	if (!command)
		return (FAILURE);
	if (command->has_heredoc && command->heredoc_fd >= 0)
	{
		if (setup_heredoc(command) == FAILURE)
			return (FAILURE);
	}
	if (!command->redirs)
		return (SUCCESS);
	return (process_redirection_list(command, command->redirs));
}
