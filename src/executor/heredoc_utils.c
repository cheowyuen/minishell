/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:36:58 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 20:25:00 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"
#include "utils.h"

static int	write_safe(int fd, const char *content, size_t len)
{
	if (write(fd, content, len) == -1)
	{
		perror("write");
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	process_unquoted_line(int fd, char *line, t_quote *quote)
{
	char	*word;
	int		i;

	i = 0;
	while (i < (int)ft_strlen(line))
	{
		if (get_word_heredoc(line, &i, quote, &word) < 0)
		{
			perror("write");
			return (FAILURE);
		}
		if (word && *word)
		{
			if (write_safe(fd, word, ft_strlen(word)) == FAILURE)
			{
				free(word);
				return (FAILURE);
			}
			free(word);
		}
	}
	return (write_safe(fd, "\n", 1));
}

static int	process_quoted_line(int fd, char *line)
{
	if (write_safe(fd, line, ft_strlen(line)) == FAILURE)
		return (FAILURE);
	return (write_safe(fd, "\n", 1));
}

static int	process_heredoc_line(int fd, char *line, t_executor *executor,
		t_quote *quote)
{
	if (!executor->commands->is_quoted)
		return (process_unquoted_line(fd, line, quote));
	else
		return (process_quoted_line(fd, line));
}

void	write_heredoc_content(int fd, char *delimiter, t_executor *executor)
{
	char	*line;
	t_quote	quote;

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
		if (process_heredoc_line(fd, line, executor, &quote) == FAILURE)
		{
			free(line);
			break ;
		}
		free(line);
	}
}
