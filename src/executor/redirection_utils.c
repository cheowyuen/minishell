/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:04:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/05 10:23:24 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	has_next(t_redir *redir, t_redir_type type)
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

int	open_redir_file(t_redir *redirs)
{
	int	fd;
	int	flags;

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

int	redirect_stdout(int fd, t_redir *redirs)
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
