/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:01:16 by cwong             #+#    #+#             */
/*   Updated: 2025/08/27 19:47:39 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"

static int	assign_new_token(char **word, char	*tmp, char	*right)
{
	char	*new_token;

	new_token = ft_strjoin(tmp, right);
	free(tmp);
	free(right);
	if (!new_token)
		return (-1);
	*word = new_token;
	return (0);
}

static int	join_token(char **word, int i, int len, t_var var)
{
	char	*tmp;
	char	*left;
	char	*right;

	left = ft_substr(*word, 0, i - var.has_braces);
	if (!left)
		return (-1);
	tmp = ft_strjoin(left, var.value);
	free(left);
	if (var.should_free)
		free(var.value);
	if (!tmp)
		return (-1);
	right = ft_substr(*word, i + len + var.has_braces,
			ft_strlen(*word) - i - len);
	if (!right)
		return (-1);
	return (assign_new_token(word, tmp, right));
}

static int	get_value(char **word, int i, int len, t_var var)
{
	char	*variable;
	if ((*word)[i + 1] != '?')
	{
		while ((*word)[i + len] && (ft_isalnum((*word)[i + len])
				|| (*word)[i + len] == '_'))
			len++;
		variable = ft_strndup(&(*word)[i + 1], len - 1);
		if (!variable)
			return (-1);
		var.value = get_env_var(var.executor, variable);
		free(variable);
		if (!var.value)
		{
			var.value = ft_strdup("");
			var.should_free = true;
		}
		else
			var.should_free = false;
		if (!var.value)
			return (-1);
	}
	return (join_token(word, i, len, var));
}

static int	get_var(char **word, int i, t_executor *executor)
{
	t_var	var;
	int		len;

	var.has_braces = false;
	var.value = NULL;
	var.should_free = false;
	var.executor = executor;
	// Check if $ is followed by valid variable character or special cases
	if (!(*word)[i + 1] || ((*word)[i + 1] != '?' && (*word)[i + 1] != '{' 
		&& !ft_isalpha((*word)[i + 1]) && (*word)[i + 1] != '_'))
		return (0); // Skip expansion, leave $ as literal
	
	if ((*word)[i + 1] == '{')
	{
		var.has_braces = true;
		i++;
		// Check for empty braces ${} 
		if ((*word)[i + 1] == '}')
			return (-1); // Error: bad substitution
	}
	if ((*word)[i + 1] == '?')
	{
		if (var.has_braces && ft_strncmp(&(*word)[i], "{?}", 3) != 0)
			return (-1);
		len = 2;
		var.value = ft_itoa(executor->last_exit_code);
		var.should_free = true;
		if (!var.value)
			return (-1);
	}
	else
		len = 1;
	if (get_value(word, i, len, var) < 0)
		return (-1);
	return (1); // Successful expansion
}

int	expand_env_var(char **word, t_executor *executor)
{
	int		i;
	int		result;
	
	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '$')
		{
			result = get_var(word, i, executor);
			if (result < 0)
				return (-1);
			if (result == 0)
				i++; // No expansion, move past this $
			else
				i = 0; // Expansion happened, restart from beginning
		}
		else
			i++;
	}
	return (0);
}
