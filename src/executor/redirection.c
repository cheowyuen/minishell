/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 20:51:07 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <stdbool.h>

static int	open_and_validate_file(const char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags);
	if (fd < 0)
		perror(filename);
	return (fd);
}

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

	fd = open_and_validate_file(redir->filename, O_RDONLY);
	if (fd < 0)
		return (FAILURE);
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

int	setup_redirection(t_command *command, t_executor *executor)
{
	t_redir	*redirs;

	if (!command)
		return (FAILURE);
	if (command->has_heredoc)
		return (setup_heredoc(command, executor));
	if (!command->redirs)
		return (SUCCESS);
	redirs = command->redirs;
	while (redirs)
	{
		if (redirs->type == REDIR_IN)
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
