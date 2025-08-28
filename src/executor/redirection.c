/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:58:47 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/28 11:45:02 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	has_next(t_redir *redir, t_redir_type type)
{
    t_redir	*tmp;

	tmp = redir->next;
    while (tmp)
	{
        if (tmp->type == type)
            return (1);
        tmp = tmp->next;
    }
    return (0);
}

static int open_redir_file(t_redir *redirs)
{
	int		fd;
	int		flags;
	
	if (redirs->type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
    fd = open(redirs->filename, flags, 0644);
    if (fd < 0)
    {
        perror(redirs->filename);
        return (-1);
    }
    return (fd);
}

static int redirect_stdout(int fd, t_redir *redirs)
{
	if (!has_next(redirs, REDIR_OUT) && !has_next(redirs, REDIR_APPEND))
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

// int	setup_output_redirection(t_command *command)
// {
// 	int			fd;
//     t_outfile	*outfiles;

//     if (!command)
//         return (FAILURE);
// 	else if (!command->outfiles)
// 		return (SUCCESS);
//     outfiles = command->outfiles;
// 	while (outfiles)
// 	{
// 		fd = open_redir_file(outfiles);
// 		if (fd < 0)
// 			return (FAILURE);
// 		if (redirect_stdout(fd, outfiles) < 0)
//             return (FAILURE);
// 		outfiles = outfiles->next;
// 	}
// 	return (SUCCESS);
// }

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
	int			fd;
	t_redir		*redirs;
	
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
