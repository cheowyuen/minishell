/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:30:56 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/02 18:31:25 by lsurco-t         ###   ########.fr       */
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

static int	check_signal_interrupt(void)
{
	if (g_signal_status == SIGNAL_INT)
	{
		return (1);
	}
	return (0);
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = SIGNAL_INT;
	rl_replace_line("", 0);
	rl_done = 1;
}

static int	read_heredoc_input(char *delimiter, bool is_quoted, 
		t_executor *executor, char **content)
{
	char	*line;
	char	*temp;
	char	*expanded_line;
	bool	is_interactive;

	*content = ft_strdup("");
	if (!*content)
		return (FAILURE);
	is_interactive = isatty(STDIN_FILENO);
	if (is_interactive)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		g_signal_status = SIGNAL_NONE;
		rl_event_hook = check_signal_interrupt;
	}
	while (1)
	{
		if (is_interactive)
			line = readline("> ");
		else
		{
			line = get_next_line(STDIN_FILENO);
			if (line && line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
		}
		if (is_interactive && (g_signal_status == SIGNAL_INT || !line))
		{
			if (line)
				free(line);
			if (g_signal_status == SIGNAL_INT)
			{
				free(*content);
				*content = NULL;
				setup_signals_prompt();
				rl_event_hook = NULL;
				return (FAILURE);
			}
			if (!line)
			{
				ft_dprintf(STDERR_FILENO, 
					"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
				setup_signals_prompt();
				rl_event_hook = NULL;
				return (SUCCESS);
			}
		}
		if (!line)
		{
			ft_dprintf(STDERR_FILENO, 
				"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
			if (is_interactive)
			{
				setup_signals_prompt();
				rl_event_hook = NULL;
			}
			return (SUCCESS);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			if (is_interactive)
			{
				setup_signals_prompt();
				rl_event_hook = NULL;
			}
			return (SUCCESS);
		}
		if (!is_quoted)
		{
			expanded_line = ft_strdup(line);
			if (expand_env_var(&expanded_line, executor) == FAILURE)
			{
				free(line);
				free(expanded_line);
				if (is_interactive)
				{
					setup_signals_prompt();
					rl_event_hook = NULL;
				}
				return (FAILURE);
			}
			temp = ft_strjoin(*content, expanded_line);
			free(expanded_line);
		}
		else
		{
			temp = ft_strjoin(*content, line);
		}
		free(line);
		if (!temp)
		{
			if (is_interactive)
			{
				setup_signals_prompt();
				rl_event_hook = NULL;
			}
			return (FAILURE);
		}
		free(*content);
		*content = temp;
		temp = ft_strjoin(*content, "\n");
		if (!temp)
		{
			if (is_interactive)
			{
				setup_signals_prompt();
				rl_event_hook = NULL;
			}
			return (FAILURE);
		}
		free(*content);
		*content = temp;
	}
}

int	process_all_heredocs(t_command *command, t_executor *executor)
{
	t_heredoc_list	*current;
	char			*content;
	int				pipefd[2];
	
	if (!command->heredoc_list)
		return (SUCCESS);
	current = command->heredoc_list;
	while (current->next)
		current = current->next;
	if (read_heredoc_input(current->delimiter, current->is_quoted, executor, &content) == FAILURE)
		return (FAILURE);
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

int	process_heredoc(t_command *command, t_executor *executor)
{
	(void)executor;
	(void)command;
	
	/* This function is kept for compatibility but heredocs are now
	 * processed after parsing is complete via process_all_heredocs() */
	return (SUCCESS);
}