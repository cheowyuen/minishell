/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 08:47:32 by cwong             #+#    #+#             */
/*   Updated: 2025/08/25 17:51:40 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && (!is_special(tokens->token[0]) || tokens->is_quoted))
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

int	is_builtin(char *cmd)
{
	char	*builtins[8];
	int		i;

	if (!cmd)
		return (0);
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(cmd, builtins[i], ft_strlen(builtins[i]) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

t_command	*create_command(void)
{
	t_command	*new_command;

	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	new_command->argv = NULL;
	new_command->infile = NULL;
	new_command->append = 0;
	new_command->has_heredoc = 0;
	new_command->heredoc_delim = NULL;
	new_command->is_quoted = false;
	new_command->is_builtin = 0;
	new_command->is_last = 0;
	new_command->pipe_out = 0;
	new_command->outfiles = NULL;
	new_command->next = NULL;
	return (new_command);
}

static void	ft_lstadd_back_redir(t_redir **lst, t_redir *new)
{
	t_redir	*end;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		new->next = NULL;
		return ;
	}
	end = *lst;
	while (end->next)
		end = end->next;
	end->next = new;
	new->next = NULL;
}

int	set_outfile(t_token **tokens, int append, t_redir **files)
{
	t_redir	*new_file;
	
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	new_file = malloc(sizeof(t_redir));
	if  (!new_file)
		return (-1);
	new_file->filename = ft_strdup((*tokens)->token); 
	if (!new_file->filename)
	{
		free(new_file);
		return (-1);
	}
	new_file->append = append;
	new_file->next = NULL;
	ft_lstadd_back_redir(files,  new_file);
	return (0);
}

int	set_infile(t_token **tokens, t_command *current)
{
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	current->infile = ft_strdup((*tokens)->token);
		// Copying the string so we can free it later
	if (!current->infile)
		return (-1);
	return (0);
}
