/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cheowyuenwong <cheowyuenwong@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 08:47:32 by cwong             #+#    #+#             */
/*   Updated: 2025/08/21 06:14:41 by cheowyuenwo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && !is_special(tokens->token[0]))
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
	new_command->outfile = NULL;
	new_command->append = 0;
	new_command->has_heredoc = 0;
	new_command->heredoc_delim = NULL;
	new_command->is_builtin = 0;
	new_command->is_last = 0;
	new_command->pipe_out = 0;
	new_command->next = NULL;
	return (new_command);
}

int	set_outfile(t_token **tokens, t_command *current, int append)
{
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	current->outfile = ft_strdup((*tokens)->token); 
	if (!current->outfile)
		return (-1);
	current->append = append;
	return (0);
}

int	set_infile(t_token **tokens, t_command *current)
{
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	current->infile = ft_strdup((*tokens)->token);  // Copying the string so we can free it later
	if (!current->infile)
		return (-1);
	return (0);
}
