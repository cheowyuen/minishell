/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:51:48 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/12 10:09:08 by cwong            ###   ########.fr       */
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
# include <stdbool.h>

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

typedef enum e_quote_type
{
	NONE,
	SINGLE,
	DOUBLE,
	DOUBLE_ENV
}	t_quote_type;

typedef struct s_token
{
	char			*token;
	t_quote_type	quote_type;
	bool			has_dollar_sign;
	struct s_token	*next;
}	t_token;

typedef struct s_var
{
	char	*value;
	bool	has_braces;
}	t_var;

typedef struct s_command
{
	char				**argv;
	char				*infile;
	char				*outfile;
	int					append;
	int					has_heredoc;
	char				*heredoc_delim;
	int					is_builtin;
	int					is_last;
	int					pipe_out;
	struct s_command	*next;	
}	t_command;


#endif
