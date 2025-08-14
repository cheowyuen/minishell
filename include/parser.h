/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:08:31 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/13 20:44:18 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_command
{
	char				**argv;
	char				*infile;
	char				*outfile;
	int					append;
	int					has_heredoc;
	char				*heredoc_delim;
	int					heredoc_expand;
	int					is_builtin;
	int					is_last;
	int					pipe_out;
	struct s_command	*next;

}						t_command;

int						is_builtin_command(const char *cmd);

#endif