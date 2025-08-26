/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/26 21:22:24 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int open_redir_file(t_outfile *outfiles)
{
	int		fd;
	int		flags;
	
	if (outfiles->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
    fd = open(outfiles->filename, flags, 0644);
    if (fd < 0)
    {
        perror(outfiles->filename);
        return (-1);
    }
    return (fd);
}

static int redirect_stdout(int fd, t_outfile *outfiles)
{
    if (!outfiles->next)
    {
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
			perror("dup2");
            close(fd);
            return (-1);
        }
    }
    close(fd);
    return (SUCCESS);
}

int	setup_output_redirection(t_command *command)
{
	int			fd;
    t_outfile	*outfiles;

    if (!command)
        return (FAILURE);
	else if (!command->outfiles)
		return (SUCCESS);
    outfiles = command->outfiles;
	while (outfiles)
	{
		fd = open_redir_file(outfiles);
		if (fd < 0)
		{
			perror(outfiles->filename);
			return (FAILURE);
		}
		if (redirect_stdout(fd, outfiles) < 0)
            return (FAILURE);
		outfiles = outfiles->next;
	}
	return (SUCCESS);
}

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
