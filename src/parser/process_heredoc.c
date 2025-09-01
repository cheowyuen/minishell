/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:30:56 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/01 18:56:37 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "utils.h"
#include "signals.h"
#include <unistd.h>
#include <stdbool.h>

static char	*append_to_content(char *content, char *line)
{
	char	*temp;
	char	*new_content;
	
	if (!content)
		temp = ft_strdup("");
	else
		temp = ft_strdup(content);
	if (!temp)
		return (NULL);
	new_content = ft_strjoin(temp, line);
	free(temp);
	if (!new_content)
		return (NULL);
	temp = ft_strjoin(new_content, "\n");
	free(new_content);
	return (temp);
}

static char	*collect_heredoc_content(char *delimiter)
{
	char	*line;
	char	*temp_line;
	char	*content;
	char	*new_content;
	
	content = NULL;
	setup_signals_heredoc();
	while (1)
	{
		// Check signal status before reading
		if (g_signal_status == SIGNAL_INT)
		{
			if (content)
				free(content);
			setup_signals_prompt();
			return (NULL);
		}
		// Read line directly here instead of helper function
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "> ", 2);
		temp_line = get_next_line(STDIN_FILENO);
		if (!temp_line)
			line = NULL;
		else
		{
			line = ft_strtrim(temp_line, "\n");
			free(temp_line);
		}
		// Check signal status immediately after readline
		if (g_signal_status == SIGNAL_INT)
		{
			if (line)
				free(line);
			if (content)
				free(content);
			setup_signals_prompt();
			return (NULL);
		}
		if (!line)
			break;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		new_content = append_to_content(content, line);
		if (content)
			free(content);
		content = new_content;
		free(line);
		if (!content)
		{
			setup_signals_prompt();
			return (NULL);
		}
	}
	setup_signals_prompt();
	return (content);
}

int	process_heredoc_during_parsing(t_command *command)
{
	if (!command || !command->has_heredoc || !command->heredoc_delim)
		return (SUCCESS);
	if (command->heredoc_content)
		free(command->heredoc_content);
	command->heredoc_content = collect_heredoc_content(command->heredoc_delim);
	if (!command->heredoc_content)
		return (FAILURE);
	return (SUCCESS);
}

