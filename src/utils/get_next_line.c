/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:57:43 by cwong             #+#    #+#             */
/*   Updated: 2025/08/21 12:31:57 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*append_buffer(char *line, char *buffer)
{
	char	*temp;

	temp = line;
	if (line)
		line = ft_strjoin(line, buffer);
	else
		line = ft_strjoin("", buffer);
	free(temp);
	return (line);
}

static char	*read_and_append(int fd, char *line)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read == -1)
	{
		free(line);
		return (NULL);
	}
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		line = append_buffer(line, buffer);
		if (!line || ft_strchr(line, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(line);
			return (NULL);
		}
	}
	return (line);
}

static char	*process_line(char **line)
{
	char	*result;
	char	*newline;
	char	*temp;

	newline = ft_strchr(*line, '\n');
	if (newline)
	{
		result = ft_substr(*line, 0, newline - *line + 1);
		temp = *line;
		*line = ft_strdup(newline + 1);
		if (!*line)
		{
			free(result);
			return (NULL);
		}
		free(temp);
	}
	else
	{
		result = ft_strdup(*line);
		free(*line);
		*line = NULL;
	}
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*line;

	if (fd == -1 || BUFFER_SIZE <= 0)
		return (NULL);
	line = read_and_append(fd, line);
	if (!line || line[0] == '\0')
	{
		free(line);
		line = NULL;
		return (NULL);
	}
	return (process_line(&line));
}
