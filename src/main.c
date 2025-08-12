/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/12 10:48:03 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include "builtin.h"
#include "parser.h"

int is_builtin_command(const char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strncmp(cmd, "echo", 4) == 0)
        return (1);
    if (ft_strncmp(cmd, "pwd", 4) == 0)
        return (1);
    if (ft_strncmp(cmd, "cd", 3) == 0)
        return (1);
    if (ft_strncmp(cmd, "export", 7) == 0)
        return (1);
    if (ft_strncmp(cmd, "unset", 6) == 0)
        return (1);
    if (ft_strncmp(cmd, "env", 4) == 0)
        return (1);
    if (ft_strncmp(cmd, "exit", 5) == 0)
        return (1);
    return (0);
}
int main(int argc, char **argv, char **envp)
{
    (void) argc;
    (void) argv;
    char *input;
    t_executor executor;
    t_command command;

    ft_bzero(&executor, sizeof(t_executor));
    ft_bzero(&command, sizeof(t_command));
    executor.env_list = NULL;
    executor.pids = NULL;
    executor.pipes = NULL;
    executor.command_count = 0;
    executor.current_command_index = 0;
    executor.last_exit_code = 0;
    executor.commands = NULL;
    add_envp_to_executor(&executor, envp);
    printf(BOLD CLR_BLUE"Welcome to Minishell!\n"RESET); // Welcome message with color
    while (1)
    {
        input = readline(CLR_GREEN PROMPT RESET); // Prompt for input with color
        if (!input)
            break;
        if (*input)
            add_history(input);
        // Here would parse the input into commands
        command.argv = ft_split(input, ' ');
        if (!command.argv[0])
        {
            free(input);
            free_split(command.argv);
            continue; // Skip empty input
        }
        if (is_builtin_command(command.argv[0]))
        {
            command.is_builtin = 1; // Mark as builtin command
        }
        else
        {
            command.is_builtin = 0; // Not a builtin command
        }
        executor.commands = &command;
        exec_cmd(&executor, executor.commands); // Execute the command
        if (command.is_builtin && ft_strcmp(command.argv[0], "exit") == 0)
        {
            free(input);
            free_split(command.argv);
            break; // Exit the shell if "exit" command
        }
        free(input);
        free_split(command.argv);
    }
    return 0;
}
