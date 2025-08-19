/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:04:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/14 11:36:28 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"

static int	create_heredoc_pipe(int pipefd[2]);
static int	fork_heredoc_writer(int pipefd[2], char *delimiter);
static int	setup_heredoc_input(int pipefd[2]);
static void	write_heredoc_content(int fd, char *delimiter);

int	setup_heredoc(t_command *command)
{
	int	pipefd[2];

	if (!command || !command->has_heredoc || !command->heredoc_delim)
		return (FAILURE);
	if (create_heredoc_pipe(pipefd) == FAILURE)
		return (FAILURE);
	if (fork_heredoc_writer(pipefd, command->heredoc_delim) == FAILURE)
		return (FAILURE);
	return (setup_heredoc_input(pipefd));
}

static int	create_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	fork_heredoc_writer(int pipefd[2], char *delimiter)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (FAILURE);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		write_heredoc_content(pipefd[1], delimiter);
		close(pipefd[1]);
		exit(0);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	return (SUCCESS);
}

static int	setup_heredoc_input(int pipefd[2])
{
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(pipefd[0]);
		return (FAILURE);
	}
	close(pipefd[0]);
	return (SUCCESS);
}

static void	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;

	setup_signals_heredoc();
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write(fd, line, ft_strlen(line)) == -1 || write(fd, "\n", 1) == -1)
		{
			perror("write");
			free(line);
			break ;
		}
		free(line);
	}
}
