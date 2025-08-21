/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:51:48 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/21 09:14:15 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Common includes throughout the project
# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// Color definitions
# define CLR_RED "\033[31m"
# define CLR_GREEN "\033[32m"
# define CLR_YELLOW "\033[33m"
# define CLR_BLUE "\033[34m"
# define CLR_MAGENTA "\033[35m"
# define CLR_CYAN "\033[36m"
# define RESET "\033[0m"
# define BOLD "\033[1m"

// Constants for exit codes
# define FAILURE 1
# define SUCCESS 0

// Prompt definition
# define PROMPT "minishell$ "

#endif
