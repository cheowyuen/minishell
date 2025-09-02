/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:41:29 by cyuen             #+#    #+#             */
/*   Updated: 2025/09/02 17:49:48 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && ((ft_strcmp(tokens->token, "|") != 0)
			|| tokens->is_quoted))
	{
		if (!tokens->is_quoted && (tokens->token[0] == '<'
				|| tokens->token[0] == '>'))
		{
			tokens = tokens->next;
			if (tokens)
				tokens = tokens->next;
			continue ;
		}
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
	new_command->append = 0;
	new_command->has_heredoc = 0;
	new_command->heredoc_delim = NULL;
	new_command->heredoc_content = NULL;
	new_command->heredoc_fd = -1;
	new_command->is_quoted = false;
	new_command->is_builtin = 0;
	new_command->is_last = 0;
	new_command->pipe_out = 0;
	new_command->heredoc_list = NULL;
	new_command->outfiles = NULL;
	new_command->redirs = NULL;
	new_command->next = NULL;
	return (new_command);
}

static void	ft_lstadd_back_outfile(t_outfile **lst, t_outfile *new)
{
	t_outfile	*end;

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

int	set_outfile(t_token **tokens, int append, t_outfile **files)
{
	t_outfile	*new_file;

	*tokens = (*tokens)->next;
	if (!*tokens)
		return (print_syntax_error("newline", NULL, 0));
	new_file = malloc(sizeof(t_outfile));
	if (!new_file)
		return (-1);
	new_file->filename = ft_strdup((*tokens)->token);
	if (!new_file->filename)
	{
		free(new_file);
		return (-1);
	}
	new_file->append = append;
	new_file->next = NULL;
	ft_lstadd_back_outfile(files, new_file);
	return (0);
}

int	set_redir(t_token **tokens, t_redir **files)
{
	t_redir	*new_file;

	new_file = malloc(sizeof(t_redir));
	if (!new_file)
		return (-1);
	if (ft_strcmp((*tokens)->token, ">>") == 0)
		new_file->type = REDIR_APPEND;
	else if ((*tokens)->token[0] == '<')
		new_file->type = REDIR_IN;
	else if ((*tokens)->token[0] == '>')
		new_file->type = REDIR_OUT;
	*tokens = (*tokens)->next;
	if (!*tokens)
		return (print_syntax_error("newline", NULL, 0));
	new_file->filename = ft_strdup((*tokens)->token);
	if (!new_file->filename)
	{
		free(new_file);
		return (-1);
	}
	new_file->next = NULL;
	ft_lstadd_back_redir(files, new_file);
	return (0);
}
