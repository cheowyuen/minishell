/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:39:50 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/02 18:46:22 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "utils.h"
#include "signals.h"
#include <unistd.h>
#include <stdbool.h>

t_heredoc_list	*add_heredoc_to_list(t_heredoc_list **list, char *delimiter, bool is_quoted)
{
	t_heredoc_list	*new_heredoc;
	t_heredoc_list	*current;

	new_heredoc = malloc(sizeof(t_heredoc_list));
	if (!new_heredoc)
		return (NULL);
	new_heredoc->delimiter = ft_strdup(delimiter);
	if (!new_heredoc->delimiter)
	{
		free(new_heredoc);
		return (NULL);
	}
	new_heredoc->is_quoted = is_quoted;
	new_heredoc->next = NULL;
	if (!*list)
	{
		*list = new_heredoc;
		return (new_heredoc);
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_heredoc;
	return (new_heredoc);
}

void	free_heredoc_list(t_heredoc_list *list)
{
	t_heredoc_list	*current;
	t_heredoc_list	*next;

	current = list;
	while (current)
	{
		next = current->next;
		if (current->delimiter)
			free(current->delimiter);
		free(current);
		current = next;
	}
}

int	create_heredoc_pipe(t_command *command, char *content)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		free(content);
		return (FAILURE);
	}
	if (write(pipefd[1], content, ft_strlen(content)) == -1)
	{
		free(content);
		close(pipefd[0]);
		close(pipefd[1]);
		return (FAILURE);
	}
	close(pipefd[1]);
	command->heredoc_fd = pipefd[0];
	if (command->heredoc_content)
		free(command->heredoc_content);
	command->heredoc_content = content;
	return (SUCCESS);
}
