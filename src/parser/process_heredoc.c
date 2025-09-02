/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:30:56 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/02 18:46:29 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "utils.h"
#include "signals.h"
#include <unistd.h>
#include <stdbool.h>


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
	
	if (!command->heredoc_list)
		return (SUCCESS);
	current = command->heredoc_list;
	while (current->next)
		current = current->next;
	if (read_heredoc_input(current->delimiter, current->is_quoted, executor, &content) == FAILURE)
		return (FAILURE);
	return (create_heredoc_pipe(command, content));
}
