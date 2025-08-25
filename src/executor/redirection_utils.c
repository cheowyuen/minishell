/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:04:30 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/25 11:16:16 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "signals.h"
#include "utils.h"
#include "minishell.h"
#include "parser.h"

static int	create_heredoc_pipe(int pipefd[2]);
static int	fork_heredoc_writer(int pipefd[2], char *delimiter, t_executor *executor);
static int	setup_heredoc_input(int pipefd[2]);
static void	write_heredoc_content(int fd, char *delimiter, t_executor *executor);

int	setup_heredoc(t_command *command, t_executor *executor)
{
	int	pipefd[2];
	
	if (!command || !command->has_heredoc || !command->heredoc_delim)
		return (FAILURE);
	if (create_heredoc_pipe(pipefd) == FAILURE)
		return (FAILURE);
	if (fork_heredoc_writer(pipefd, command->heredoc_delim, executor) == FAILURE)
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

static int	fork_heredoc_writer(int pipefd[2], char *delimiter, t_executor *executor)
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
		setup_signals_heredoc();
		write_heredoc_content(pipefd[1], delimiter, executor);
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		close(pipefd[1]); 
		setup_signals_ignore();
		if (waitpid(pid, &status, 0) == WAITPID_ERROR)
		{
			perror("waitpid");
			return (FAILURE);
		}
		setup_signals_prompt();
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

static void	write_heredoc_content(int fd, char *delimiter, t_executor *executor)
{
	char	*line;
	t_quote	quote;
	char	*word;
	int		i;

	quote.executor = executor;
	quote.is_heredoc = true;
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
		i = 0;
		if (!executor->commands->is_quoted)
		{
			if (get_word(line, &i, &quote, &word) < 0)
			{
				perror("write");
				free(line);
				break ;
			}
			if (write(fd, word, ft_strlen(word)) == -1 || write(fd, "\n", 1) == -1)
			{
				perror("write");
				free(line);
				break ;
			}
			free(word);
		}
		else if (write(fd, line, ft_strlen(line)) == -1 || write(fd, "\n", 1) == -1)
		{
			perror("write");
			free(line);
			break ;
		}
		free(line);
	}
}
