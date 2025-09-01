/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:41:50 by cyuen             #+#    #+#             */
/*   Updated: 2025/09/01 22:36:47 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	free_tokens(t_token **tokens)
{
	if (tokens && *tokens)
		ft_lstclear(tokens);
	return (-1);
}

void	free_commands(t_command **commands)
{
	t_command	*temp;
	int			i;
	t_outfile	*outfiles;
	t_redir		*redirs;
	t_outfile	*next;
	t_redir		*next_redir;

	if (!commands || !*commands)
		return ;
	while (*commands)
	{
		temp = *commands;
		*commands = (*commands)->next;
		i = 0;
		if (temp->argv)
		{
			while (temp->argv[i])
			{
				free(temp->argv[i]);
				i++;
			}
			free(temp->argv);
		}
		redirs = temp->redirs;
		while (redirs)
		{
			next_redir = redirs->next;
			if (redirs->filename)
				free(redirs->filename);
			free(redirs);
			redirs = next_redir;
		}
		outfiles = temp->outfiles;
		while (outfiles)
		{
			next = outfiles->next;
			if (outfiles->filename)
				free(outfiles->filename);
			free(outfiles);
			outfiles = next;
		}
		if (temp->heredoc_delim)
			free(temp->heredoc_delim);
		if (temp->heredoc_content)
			free(temp->heredoc_content);
		if (temp->heredoc_fd >= 0)
			close(temp->heredoc_fd);
		free(temp);
	}
	*commands = NULL;
}

int	free_tokens_commands(t_token **tokens, t_command **commands)
{
	free_tokens(tokens);
	free_commands(commands);
	return (-1);
}

int	free_tokens_input(t_token **tokens, char *input, int flag)
{
	if (flag)
		free_tokens(tokens);
	else if (!flag)
	{
		if (input)
		{
			free_tokens(tokens);
			free(input);
		}
	}
	return (-1);
}
