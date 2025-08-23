/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:01:16 by cwong             #+#    #+#             */
/*   Updated: 2025/08/19 11:39:09 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"

static int	assign_new_token(t_token *tokens, char *tmp, char *right)
{
	char	*new_token;

	new_token = ft_strjoin(tmp, right);
	free(tmp);
	free(right);
	if (!new_token)
		return (free_tokens(&tokens));
	free(tokens->token);
	tokens->token = new_token;
	return (0);
}

static int	join_token(t_token *tokens, int i, int len, t_var var)
{
	char	*tmp;
	char	*left;
	char	*right;

	left = ft_substr(tokens->token, 0, i - var.has_braces);
	if (!left)
		return (free_tokens(&tokens));
	tmp = ft_strjoin(left, var.value);
	free(left);
	if (var.should_free)
		free(var.value);
	if (!tmp)
		return (free_tokens(&tokens));
	right = ft_substr(tokens->token, i + len + var.has_braces,
			ft_strlen(tokens->token) - i - len);
	if (!right)
		return (free_tokens(&tokens));
	return (assign_new_token(tokens, tmp, right));
}

static int	get_value(t_token *tokens, int i, int len, t_var var)
{
	char	*variable;

	if (tokens->token[i + 1] != '?')
	{
		while (tokens->token[i + len] && (ft_isalnum(tokens->token[i + len])
				|| tokens->token[i + len] == '_'))
			len++;
		variable = ft_strndup(&tokens->token[i + 1], len - 1);
		if (!variable)
			return (free_tokens(&tokens));
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
			return (free_tokens(&tokens));
	}
	return (join_token(tokens, i, len, var));
}

static int	get_var(t_token *tokens, int i, t_shell *shell,
		t_executor *executor)
{
	t_var	var;
	int		len;

	var.has_braces = false;
	var.value = NULL;
	var.should_free = false;
	var.executor = executor;
	if (tokens->token[i + 1] == '{')
	{
		var.has_braces = true;
		i++;
	}
	if (tokens->token[i + 1] == '?')
	{
		if (var.has_braces && ft_strncmp(&tokens->token[i], "{?}", 4) != 0)
			return (free_tokens(&tokens));
		len = 2;
		shell->exit_code = 0;
		var.value = ft_itoa(shell->exit_code);
		var.should_free = true;
		if (!var.value)
			return (free_tokens(&tokens));
	}
	else
		len = 1;
	return (get_value(tokens, i, len, var));
}

int	expand_env_var(t_token *tokens, t_shell *shell, t_executor *executor)
{
	int	i;

	while (tokens)
	{
		if (tokens->quote_type == DOUBLE_ENV || tokens->quote_type == NONE)
		{
			i = 0;
			while (tokens->token[i])
			{
				if (tokens->token[i] == '$')
				{
					if (get_var(tokens, i, shell, executor))
						return (-1);
					i = 0;
				}
				else
					i++;
			}
		}
		tokens = tokens->next;
	}
	return (0);
}
