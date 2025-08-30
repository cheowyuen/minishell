/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 19:43:01 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	setup_input_redirection(t_command *command, t_executor *executor)
{
	int			fd;
	t_infile	*infiles;

	if (!command)
		return (FAILURE);
	if (command->has_heredoc)
		return (setup_heredoc(command, executor));
	if (!command->infiles)
		return (SUCCESS);
	infiles = command->infiles;
	while (infiles)
	{
		fd = open(infiles->filename, O_RDONLY);
		if (fd < 0)
		{
			perror(infiles->filename);
			return (FAILURE);
		}
		if (!infiles->next)
		{
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return (FAILURE);
			}
		}
		close(fd);
		infiles = infiles->next;
	}
	return (SUCCESS);
}

int	setup_redirection(t_command *command, t_executor *executor)
{
	int		fd;
	t_redir	*redirs;

	(void)executor; // Not used in this function
	if (!command)
		return (FAILURE);
	// No need to handle heredoc here - that's handled by setup_input_redirection
	if (!command->redirs)
		return (SUCCESS);
	redirs = command->redirs;
	while (redirs)
	{
		if (redirs->type == REDIR_IN)
		{
			fd = open(redirs->filename, O_RDONLY);
			if (fd < 0)
			{
				perror(redirs->filename);
				return (FAILURE);
			}
			if (!has_next(redirs, REDIR_IN))
			{
				if (dup2(fd, STDIN_FILENO) < 0)
				{
					perror("dup2");
					close(fd);
					return (FAILURE);
				}
			}
			close(fd);
		}
		else if (redirs->type == REDIR_OUT || redirs->type == REDIR_APPEND)
		{
			fd = open_redir_file(redirs);
			if (fd < 0)
				return (FAILURE);
			if (redirect_stdout(fd, redirs) < 0)
				return (FAILURE);
		}
		redirs = redirs->next;
	}
	return (SUCCESS);
}
