/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:30:56 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/01 16:53:31 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "utils.h"
#include "signals.h"
#include <unistd.h>
#include <stdbool.h>

static char	*read_heredoc_line(void)
{
	char	*line;
	char	*temp_line;
	
	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
	{
		temp_line = get_next_line(STDIN_FILENO);
		if (!temp_line)
			return (NULL);
		line = ft_strtrim(temp_line, "\n");
		free(temp_line);
	}
	return (line);
}

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
	char	*content;
	char	*new_content;
	
	content = NULL;
	setup_signals_heredoc();
	while (1)
	{
		line = read_heredoc_line();
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
			return (NULL);
	}
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

