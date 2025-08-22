/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:04:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/22 07:15:01 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"
#include "utils.h"
#include "minishell.h"

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

int heredoc_hook(void)
{
    if (g_signal_status == SIGINT)
    {
        rl_done = 1;
        g_signal_status = 0;
    }
    return 0;
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
	else
	{
		close(pipefd[1]);
		printf("Before waitpid\n");
		if (waitpid(pid, &status, 0) == WAITPID_ERROR)
		{
			perror("waitpid");
			return (FAILURE);
		}
		printf("After waitpid\n");
		return (status);
	}
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
	
	rl_event_hook = heredoc_hook;
	signal(SIGINT, sigint_handler); 
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (g_signal_status == 2)
		{
			free(line);
			break;
		}
		if (!line)
			break ;	
		if (ft_strcmp(line, delimiter) == 0 ||
		    (line[ft_strlen(line) - 1] == '\n' &&
		     ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 &&
		     line[ft_strlen(delimiter)] == '\n')) //remove trailing newline
		{
			free(line);
			break ;
		}
		if (write(fd, line, ft_strlen(line)) == -1)
		{
			perror("write");
			free(line);
			break ;
		}
		free(line);
	}
	rl_event_hook = NULL;
}
