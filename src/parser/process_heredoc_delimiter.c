/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc_delimiter.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:00:00 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/30 20:31:51 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "environment.h"
#include "minishell.h"
#include "parser.h"
#include "utils.h"

static char	*strip_quotes(const char *str);
static char	*expand_delimiter(char *str, t_executor *executor);

char	*process_heredoc_delimiter(const char *delimiter, bool is_quoted, 
	t_executor *executor)
{
	char	*processed_delimiter;
	char	*temp;

	if (!delimiter)
		return (NULL);
	processed_delimiter = strip_quotes(delimiter);
	if (!processed_delimiter)
		return (NULL);
	if (!is_quoted)
	{
		temp = expand_delimiter(processed_delimiter, executor);
		free(processed_delimiter);
		if (!temp)
			return (NULL);
		processed_delimiter = temp;
	}
	
	return (processed_delimiter);
}

static char	*strip_quotes(const char *str)
{
	int		len;
	char	quote_char;

	if (!str || !*str)
		return (ft_strdup(""));
	len = ft_strlen(str);
	if (len >= 2 && (str[0] == '"' || str[0] == '\''))
	{
		quote_char = str[0];
		if (str[len - 1] == quote_char)
		{
			return (ft_substr(str, 1, len - 2));
		}
	}
	return (ft_strdup(str));
}

static char	*expand_delimiter(char *str, t_executor *executor)
{
	char	*expanded;
	
	if (!str)
		return (NULL);
	expanded = ft_strdup(str);
	if (!expanded)
		return (NULL);
	if (expand_env_var(&expanded, executor) < 0)
	{
		free(expanded);
		return (NULL);
	}
	
	return (expanded);
}
