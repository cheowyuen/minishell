/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:29 by cwong             #+#    #+#             */
/*   Updated: 2025/08/15 09:14:29 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

//for debugging
void print_commands(t_command *cmd)
{
    int i;
    int cmd_num = 1;

    while (cmd)
    {
        printf("Command #%d:\n", cmd_num++);
        
        // Print argv array
        printf("  argv: ");
        if (cmd->argv)
        {
            for (i = 0; cmd->argv[i] != NULL; i++)
                printf("\"%s\" ", cmd->argv[i]);
        }
        else
            printf("(null)");
        printf("\n");
        
        // Print redirection info
        printf("  infile: %s\n", cmd->infile ? cmd->infile : "(none)");
        printf("  outfile: %s\n", cmd->outfile ? cmd->outfile : "(none)");
        printf("  append: %d\n", cmd->append);
        
        // Heredoc info
        printf("  has_heredoc: %d\n", cmd->has_heredoc);
        printf("  heredoc_delim: %s\n", cmd->heredoc_delim ? cmd->heredoc_delim : "(none)");
        
        // Pipe and flags
        printf("  pipe_out: %d\n", cmd->pipe_out);
		printf("  is_builtin: %d\n", cmd->is_builtin);
        printf("  is_last: %d\n", cmd->is_last);
        
        printf("-------------------------\n");
        cmd = cmd->next;
    }
}

static int	handle_commands(t_token **tokens, t_command *current
	, t_command *commands)
{
	int			count;
	int			i;

	count = count_tokens(*tokens);
	current->argv = ft_calloc(count + 1, sizeof(char *));
	if (!current->argv)
		return (free_tokens_commands(tokens, &commands));
	i = 0;
	while (i < count)
	{
		current->argv[i] = ft_strdup((*tokens)->token);
		if (!current->argv[i])
			return (free_tokens_commands(tokens, &commands));
		if (i == 0 && is_builtin((*tokens)->token))
			current->is_builtin = 1;
		*tokens = (*tokens)->next;
		i++;
	}
	current->argv[i] = NULL;
	return (0);
}

static int	handle_special(t_token **tokens, t_command **current
	, t_command *commands)
{
	t_command	*new_command;

	if (ft_strcmp((*tokens)->token, "<<") == 0)
	{
		*tokens = (*tokens)->next;
		(*current)->has_heredoc = 1;
		(*current)->heredoc_delim = (*tokens)->token;
	}
	else if (ft_strcmp((*tokens)->token, ">>") == 0)
		set_outfile(tokens, *current, 1);
	else if ((*tokens)->token[0] == '<')
		set_infile(tokens, *current);
	else if ((*tokens)->token[0] == '>')
		set_outfile(tokens, *current, 0);
	else
	{
		(*current)->pipe_out = 1;
		new_command = create_command();
		if (!new_command)
			return (free_tokens_commands(tokens, &commands));
		(*current)->next = new_command;
		*current = (*current)->next;
	}
	*tokens = (*tokens)->next;
	return (0);
}

static int	process_tokens(t_token *tokens, t_command *commands)
{
	t_command	*current;

	current = commands;
	while (tokens)
	{
		if (is_special(tokens->token[0]))
		{
			if (handle_special(&tokens, &current, commands) < 0)
				return (-1);
		}
		else
		{
			if (handle_commands(&tokens, current, commands) < 0)
				return (-1);
		}
	}
	current->is_last = 1;
	return (0);
}

int	parse_commands(t_token *tokens)
{
	t_command	*new_command;

	new_command = create_command();
	if (!new_command)
		return (free_tokens(&tokens));
	if (process_tokens(tokens, new_command) < 0)
		return (-1);
	//for debugging
	print_commands(new_command);
	return (0);
}
