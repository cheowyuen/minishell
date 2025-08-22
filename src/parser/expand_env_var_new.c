/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var_new.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:01:16 by cwong             #+#    #+#             */
/*   Updated: 2025/08/22 11:23:41 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "executor.h"
#include "builtin.h"

static int	assign_new_token_new(char **word, char	*tmp, char	*right)
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

static int	join_token_new(char **word, int i, int len, t_var var)
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
	return (assign_new_token_new(word, tmp, right));
}

static int	get_value_new(char **word, int i, int len, t_var var)
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
	return (join_token_new(word, i, len, var));
}

static int	get_var_new(char **word, int i, t_shell *shell, t_executor *executor)
{
	t_var	var;
	int		len;

	var.has_braces = false;
	var.value = NULL;
	var.should_free = false;
	var.executor = executor;
	if ((*word)[i + 1] == '{')
	{
		var.has_braces = true;
		i++;
	}
	if ((*word)[i + 1] == '?')
	{
		if (var.has_braces && ft_strncmp(word[i], "{?}", 4) != 0)
			return (-1);
		len = 2;
		var.value = ft_itoa(shell->exit_code);
		var.should_free = true;
		if (!var.value)
			return (-1);
	}
	else
		len = 1;
	return (get_value_new(word, i, len, var));
}

int	expand_env_var_new(char **word, t_shell *shell, t_executor *executor)
{
	int		i;
	
	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '$')
		{
			if (get_var_new(word, i, shell, executor) < 0)
				return (-1);
			i = 0;
		}
		else
			i++;
	}
	return (0);
}
