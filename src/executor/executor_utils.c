/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:02:01 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/05 10:22:27 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "environment.h"
#include "signals.h"
#include <sys/stat.h>

static void	execve_execution(t_executor *executor, t_command *command,
				char *cmd_path);

void	count_commands(t_command *command, t_executor *executor)
{
	int	count;

	if (!executor || !command)
		return ;
	count = 0;
	executor->command_count = 0;
	while (command)
	{
		count++;
		command = command->next;
	}
	executor->command_count = count;
}

void	exec_external_command(t_executor *executor, t_command *command)
{
	char		*cmd_path;
	struct stat	st;

	if (!command->argv || !command->argv[0] || !command->argv[0][0])
	{
		ft_dprintf(STDERR_FILENO, "minishell: : command not found\n");
		exit(EXIT_CMD_NOT_FOUND);
	}
	cmd_path = find_command_path(executor, command->argv[0]);
	if (!cmd_path)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n",
			command->argv[0]);
		free_inherited_state_for_child(executor, executor->commands);
		exit(EXIT_CMD_NOT_FOUND);
	}
	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: Is a directory\n",
			command->argv[0]);
		free(cmd_path);
		free_inherited_state_for_child(executor, executor->commands);
		exit(EXIT_CMD_NOT_EXECUTABLE);
	}
	execve_execution(executor, command, cmd_path);
}

static void	execve_execution(t_executor *executor, t_command *command,
		char *cmd_path)
{
	char	**envp;

	add_var_to_list(executor, "_", cmd_path);
	envp = build_env_array(executor);
	if (!envp)
	{
		free(cmd_path);
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, command->argv, envp);
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", command->argv[0],
		strerror(errno));
	free(cmd_path);
	free_str_array(envp);
	if (errno == EACCES || errno == ENOTDIR)
		exit(EXIT_CMD_NOT_EXECUTABLE);
	else
		exit(EXIT_CMD_NOT_FOUND);
}

void	add_envp_to_executor(t_executor *executor, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (add_env_variable(executor, envp[i]) == FAILURE)
		{
			print_error_stderr("Error adding environment variable\n");
			return ;
		}
		i++;
	}
}

int	handle_parent_process(pid_t pid)
{
	int	status;

	setup_signals_ignore();
	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		setup_signals_prompt();
		return (FAILURE);
	}
	setup_signals_prompt();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		return (EXIT_SIGNAL_BASE + WTERMSIG(status));
	}
	return (FAILURE);
}
