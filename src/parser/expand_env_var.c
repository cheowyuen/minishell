/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:01:16 by cwong             #+#    #+#             */
/*   Updated: 2025/08/14 14:14:40 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/executor.h"

static int	assign_new_token(t_token *tokens, char	*tmp, char	*right)
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

static int	get_value(t_token *tokens, int i, int len, t_var var)
{
	int		braces_len;
	char	*tmp;
	char	*left;          
	char	*right;

	braces_len = 0;
	if (var.has_braces)
		braces_len = 2;
	left = ft_substr(tokens->token, 0, i - var.has_braces);
	if (!left)
		return (free_tokens(&tokens));
	tmp = ft_strjoin(left, var.value);
	free(left);
	if (!tmp)
		return (free_tokens(&tokens));
	right =  ft_substr(tokens->token, i + len + var.has_braces, ft_strlen(tokens->token) - i - len);
	if (!right)
		return (free_tokens(&tokens));
	return (assign_new_token(tokens, tmp, right));
}

static int	get_var(t_token *tokens, int i, t_shell *shell, t_env **env)
{
	t_var	var;
	int		len;
	char	*variable;
	
	var.has_braces = false;
	if (tokens->token[i + 1] == '{')
	{
		var.has_braces = true;
		i++;
	}
	if (tokens->token[i + 1] == '?' )
	{
		if (var.has_braces && ft_strncmp(&tokens->token[i], "{?}", 4) != 0)
			return (free_tokens(&tokens));
		len = 2;
		var.value = ft_itoa(shell->exit_code);
		if (!var.value)
			return (free_tokens(&tokens));
	}
	else
	{
		len = 1;
		while (tokens->token[i + len] && (ft_isalnum(tokens->token[i + len])
			|| tokens->token[i + len] == '_'))
			len++;
		variable = ft_strndup(&tokens->token[i + 1], len - 1);
		if (!variable)
			return (free_tokens(&tokens));
		var.value = getenv(variable);
		free(variable);
		if (!var.value)
			var.value = ft_strdup("");
		if (!var.value)
			return (free_tokens(&tokens));
	}
	return (get_value(tokens, i, len, var));
}

int	expand_env_var(t_token *tokens, t_shell *shell, t_env **env)
{
	int		i;
	
	while (tokens)
	{
		if (tokens->quote_type == DOUBLE_ENV)
		{
			i = 0;
			while (tokens->token[i])
			{
				if (tokens->token[i] == '$')
				{
					if (get_var(tokens, i, shell, env))
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
