/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:39:50 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/02 20:04:53 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "signals.h"
#include "utils.h"
#include <stdbool.h>
#include <unistd.h>

static char	*read_heredoc_line(bool is_interactive)
{
	char	*line;

	if (is_interactive)
		line = readline("> ");
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
	}
	return (line);
}

static int	handle_signal_or_eof(char *line, char *delimiter,
		bool is_interactive, char **content)
{
	if (is_interactive && (g_signal_status == SIGNAL_INT || !line))
	{
		if (line)
			free(line);
		if (g_signal_status == SIGNAL_INT)
		{
			free(*content);
			*content = NULL;
			return (cleanup_heredoc_signals(is_interactive, FAILURE));
		}
		if (!line)
			return (cleanup_eof_warning(delimiter, is_interactive));
	}
	if (!line)
		return (cleanup_eof_warning(delimiter, is_interactive));
	return (-1);
}

static int	append_newline_to_content(char **content, char *temp)
{
	char	*new_content;

	free(*content);
	*content = temp;
	new_content = ft_strjoin(*content, "\n");
	if (!new_content)
		return (FAILURE);
	free(*content);
	*content = new_content;
	return (SUCCESS);
}

static int	process_heredoc_line_content(char *line, bool is_quoted,
		t_executor *executor, char **content)
{
	char	*temp;
	char	*expanded_line;

	if (!is_quoted)
	{
		expanded_line = ft_strdup(line);
		if (expand_env_var(&expanded_line, executor) == FAILURE)
		{
			free(line);
			free(expanded_line);
			return (FAILURE);
		}
		temp = ft_strjoin(*content, expanded_line);
		free(expanded_line);
	}
	else
		temp = ft_strjoin(*content, line);
	free(line);
	if (!temp)
		return (FAILURE);
	return (append_newline_to_content(content, temp));
}

int	read_heredoc_input(char *delimiter, bool is_quoted, t_executor *executor,
		char **content)
{
	char	*line;
	bool	is_interactive;
	int		result;

	*content = ft_strdup("");
	if (!*content)
		return (FAILURE);
	is_interactive = isatty(STDIN_FILENO);
	setup_heredoc_signals(is_interactive);
	while (1)
	{
		line = read_heredoc_line(is_interactive);
		result = handle_signal_or_eof(line, delimiter, is_interactive, content);
		if (result != -1)
			return (result);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (cleanup_heredoc_signals(is_interactive, SUCCESS));
		}
		if (process_heredoc_line_content(line, is_quoted, executor, content) == FAILURE)
			return (cleanup_heredoc_signals(is_interactive, FAILURE));
	}
}
