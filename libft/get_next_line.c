/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:30:25 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/05/16 10:46:03 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*partial_string(char *buffer)
{
	char	*str;
	size_t	i;

	i = 0;
	if (!buffer || buffer[0] == '\0')
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	str = malloc((i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		str[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		str[i++] = '\n';
	str[i] = '\0';
	return (str);
}

static char	*save_leftover(char *buffer)
{
	char	*leftover;
	size_t	i;
	size_t	x;

	i = 0;
	x = 0;
	if (!buffer)
		return (NULL);
	while (buffer[x] && buffer[x] != '\n')
		x++;
	if (buffer[x] == '\n')
		x++;
	if (!buffer[x])
		return (NULL);
	leftover = malloc(((ft_strlen(buffer) - x) + 1) * sizeof(char));
	if (!leftover)
		return (NULL);
	while (buffer[x])
		leftover[i++] = buffer[x++];
	leftover[i] = '\0';
	return (leftover);
}

static int	read_to_line(int fd, char **total_line)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	char	*temp;

	if (*total_line == NULL)
		*total_line = ft_strdup("");
	if (!*total_line)
		return (-1);
	bytes_read = 1;
	while (!ft_strchr(*total_line, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (-1);
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(*total_line, buffer);
		if (!temp)
			return (-1);
		free(*total_line);
		*total_line = temp;
	}
	return (bytes_read);
}

static char	*final_line(char **total_line)
{
	char	*line;

	if (*total_line && **total_line)
	{
		line = ft_strdup(*total_line);
		free(*total_line);
		*total_line = NULL;
		return (line);
	}
	free(*total_line);
	*total_line = NULL;
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*total_line;
	char		*line;
	char		*leftover;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (read_to_line(fd, &total_line) <= 0)
		return (final_line(&total_line));
	line = partial_string(total_line);
	if (!line)
	{
		free(total_line);
		total_line = NULL;
		return (NULL);
	}
	leftover = save_leftover(total_line);
	free(total_line);
	if (!leftover)
	{
		total_line = NULL;
	}
	else
		total_line = leftover;
	return (line);
}
