/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:20:00 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 15:26:47 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"

static int	expand_tilde_word(char **word, char *home_path)
{
	char	*new_word;
	char	*rest_of_path;

	if ((*word)[1] == '\0')
	{
		free(*word);
		*word = ft_strdup(home_path);
		if (!*word)
			return (-1);
	}
	else if ((*word)[1] == '/')
	{
		rest_of_path = ft_strdup(&(*word)[1]);
		if (!rest_of_path)
			return (-1);
		new_word = ft_strjoin(home_path, rest_of_path);
		free(rest_of_path);
		if (!new_word)
			return (-1);
		free(*word);
		*word = new_word;
	}
	return (0);
}

int	expand_tilde(char **word, t_executor *executor)
{
	char	*home_path;

	if (!word || !*word || (*word)[0] != '~')
		return (0);
	home_path = get_env_var(executor, "HOME");
	if (!home_path)
		return (0);
	if (expand_tilde_word(word, home_path) < 0)
		return (0);
	return (0);
}
