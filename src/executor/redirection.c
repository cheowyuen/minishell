/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/21 08:50:50 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	setup_input_redirection(t_command *command)
{
	int	fd;
	
	if (!command)
		return (SUCCESS);
	if (command->has_heredoc)
		return (setup_heredoc(command));
	if (!command->infile)
		return (SUCCESS);
	fd = open(command->infile, O_RDONLY);
	if (fd < 0)
	{
		perror(command->infile);
		return (FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (FAILURE);
	}
	close(fd);
	return (SUCCESS);
}

int	setup_output_redirection(t_command *command)
{
	int	fd;
	int	flags;

	if (!command || !command->outfile)
		return (SUCCESS);
	if (command->append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(command->outfile, flags, 0644);
	if (fd < 0)
	{
		perror(command->outfile);
		return (FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (FAILURE);
	}
	close(fd);
	return (SUCCESS);
}
