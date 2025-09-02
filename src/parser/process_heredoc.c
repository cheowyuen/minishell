/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:30:56 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/02 19:35:05 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "signals.h"
#include "utils.h"
#include <stdbool.h>
#include <unistd.h>

int	process_all_heredocs(t_command *command, t_executor *executor)
{
	t_heredoc_list	*current;
	char			*content;

	if (!command->heredoc_list)
		return (SUCCESS);
	current = command->heredoc_list;
	while (current)
	{
		if (read_heredoc_input(current->delimiter, current->is_quoted, executor,
				&content) == FAILURE)
			return (FAILURE);
		if (current->next)
		{
			free(content);
			current = current->next;
		}
		else
			return (create_heredoc_pipe(command, content));
	}
	return (SUCCESS);
}

t_heredoc_list	*add_heredoc_to_list(t_heredoc_list **list, char *delimiter,
		bool is_quoted)
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

int	cleanup_eof_warning(char *delimiter, bool is_interactive)
{
	ft_dprintf(STDERR_FILENO,
				"minishell: warning: "
				"here-document delimited by "
				"end-of-file (wanted `%s')\n",
				delimiter);
	cleanup_heredoc_signals(is_interactive);
	return (SUCCESS);
}
