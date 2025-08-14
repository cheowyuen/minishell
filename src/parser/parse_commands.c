/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:29 by cwong             #+#    #+#             */
/*   Updated: 2025/08/14 08:42:11 by cwong            ###   ########.fr       */
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

static int	is_builtin(char *cmd)
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

static t_command	*create_command()
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

static int	process_tokens(t_token *tokens, t_command *commands)
{
	t_command	*current;
	t_command	*new_command;
	t_token		*temp;
	int			count;
	int			i;

	current = commands;
	while (tokens)
	{
		if (is_special(tokens->token[0]))
		{
			if (ft_strcmp(tokens->token, "<<") == 0)
			{
				tokens = tokens->next;
				current->has_heredoc = 1;
				current->heredoc_delim = tokens->token;
			}
			else if (ft_strcmp(tokens->token, ">>") == 0)
			{
				tokens = tokens->next;
				current->outfile = tokens->token;
				current->append = 1;
			}
			else if (tokens->token[0] == '<')
			{
				tokens = tokens->next;
				current->infile = tokens->token;
			}
			else if (tokens->token[0] == '>')
			{
				tokens = tokens->next;
				current->outfile = tokens->token;
				current->append = 0;
			}
			else 
			{
				current->pipe_out = 1;
				new_command = create_command();
				if (!new_command)
					return (free_tokens_commands(&tokens, &commands));
				current->next = new_command;
				current = current->next;
			}
			tokens = tokens->next;
		}
		else
		{
			count = 0;
			temp = tokens;
			while (temp && !is_special(temp->token[0]))
			{
				count++;
				temp = temp->next;
			}
			current->argv = ft_calloc(count + 1, sizeof(char *));
			if (!current->argv)
				return (free_tokens_commands(&tokens, &commands));
			i = 0;
			while (i < count)
			{
				current->argv[i] = ft_strdup(tokens->token);
				if (!current->argv[i])
					return (free_tokens_commands(&tokens, &commands));
				if (i == 0 && is_builtin(tokens->token))
						current->is_builtin = 1;
				tokens = tokens->next;
				i++;
			}
			current->argv[i] = NULL;
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
