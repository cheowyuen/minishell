/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc_delimiter.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:00:00 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/09/02 23:26:07 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "executor.h"
#include "minishell.h"
#include "parser.h"
#include "utils.h"

static char	*strip_quotes(const char *str);

char	*process_heredoc_delimiter(const char *delimiter)
{
	char	*processed_delimiter;

	if (!delimiter)
		return (NULL);
	processed_delimiter = strip_quotes(delimiter);
	if (!processed_delimiter)
		return (NULL);
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
