/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/08/01 18:11:15 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
    char *input;

	printf(BOLD CLR_BLUE"Welcome to Minishell!\n"RESET);
    while (1)
    {
        input = readline(PROMPT);
        if (!input)
            break;
        if (*input)
            add_history(input);
        free(input);
    }
    return 0;
}
