/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:51:48 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/01 18:10:54 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define FAILURE 1
# define SUCCESS 0

# define CLR_RED "\033[31m"
# define CLR_GREEN "\033[32m"
# define CLR_YELLOW "\033[33m"
# define CLR_BLUE "\033[34m"
# define CLR_MAGENTA "\033[35m"
# define CLR_CYAN "\033[36m"
# define RESET "\033[0m"
# define BOLD "\033[1m"

# define PROMPT "minishell$ "


#endif
