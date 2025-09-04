/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:20:00 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/04 12:14:18 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"

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

int	expand_word_if_needed(char **word, bool has_dollar, t_executor *executor)
{
	if (expand_tilde(word, executor) < 0)
		return (-1);
	if (has_dollar)
	{
		if (expand_env_var(word, executor) < 0)
			return (-1);
	}
	return (0);
}

int	handle_exit_code(const char *word, int i, t_var *var, t_executor *executor)
{
	if (word[i + 1] == '?')
	{
		if (var->has_braces && ft_strncmp(&word[i], "{?}", 3) != 0)
			return (-1);
		var->value = ft_itoa(executor->last_exit_code);
		if (!var->value)
			return (-1);
		var->should_free = true;
		return (2);
	}
	return (1);
}
