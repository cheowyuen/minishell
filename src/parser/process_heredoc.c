/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:30:56 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/02 08:27:39 by lsurco-t         ###   ########.fr       */
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
	while (1)
	{
		// Check signal status before reading
		if (g_signal_status == SIGNAL_INT)
		{
			if (content)
				free(content);
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
			return (NULL);
		}
		if (!line)
		{
			// Handle EOF (Ctrl+D) - print newline and break
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
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
			return (NULL);
		}
	}
	return (content);
}

int	process_heredoc(t_command *command, t_executor *executor)
{
	char	*expanded_content;

	if (!command || !command->has_heredoc || !command->heredoc_delim)
		return (SUCCESS);
	if (command->heredoc_content)
		free(command->heredoc_content);
	command->heredoc_content = collect_heredoc_content(command->heredoc_delim);
	if (!command->heredoc_content)
		return (FAILURE);
	if (!command->is_quoted && command->heredoc_content)
	{
		expanded_content = ft_strdup(command->heredoc_content);
		if (!expanded_content)
		{
			free(command->heredoc_content);
			command->heredoc_content = NULL;
			return (FAILURE);
		}
		if (expand_env_var(&expanded_content, executor) < 0)
		{
			free(expanded_content);
			free(command->heredoc_content);
			command->heredoc_content = NULL;
			return (FAILURE);
		}
		free(command->heredoc_content);
		command->heredoc_content = expanded_content;
	}
	return (SUCCESS);
}

