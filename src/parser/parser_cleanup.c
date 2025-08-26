/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:30:25 by cwong             #+#    #+#             */
/*   Updated: 2025/08/26 21:07:14 by cheowyuenwo      ###   ########.fr       */
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
	t_infile	*infiles;
	t_outfile	*outfiles;
	t_outfile	*next;
	t_infile	*next_infile;

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
		infiles = temp->infiles;
		while (infiles)
		{
			next_infile = infiles->next;
			if (infiles->filename)
				free(infiles->filename);
			free(infiles);
			infiles = next_infile;
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
