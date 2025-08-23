/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/23 20:40:48 by cheowyuenwo      ###   ########.fr       */
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

static int open_redir_file(t_redir *redir)
{
	int		fd;
	int		flags;
	
	if (redir->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
    fd = open(redir->filename, flags, 0644);
    if (fd < 0)
    {
        perror(redir->filename);
        return (-1);
    }
    return (fd);
}

static int redirect_stdout(int fd, t_redir *redir)
{
    if (!redir->next)
    {
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            close(fd);
            return (-1);
        }
    }
    close(fd);
    return (SUCCESS);
}

int	setup_output_redirection(t_command *command)
{
	int		fd;
    t_redir *redir;

    if (!command)
        return (FAILURE);
	if (!command->outfiles)
		return (SUCCESS);
    redir = command->outfiles;
	while (redir)
	{
		fd = open_redir_file(redir);
		if (fd < 0)
		{
			perror(redir->filename);
			return (FAILURE);
		}
		if (redirect_stdout(fd, redir) < 0)
            return (FAILURE);
		redir = redir->next;
	}
	return (SUCCESS);
}
