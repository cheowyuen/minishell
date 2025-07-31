/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsurco-t <lsurco-t@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:49:37 by lsurco-t          #+#    #+#             */
/*   Updated: 2025/07/31 19:50:58 by lsurco-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
    char *input;

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
            break;
        if (*input)
            add_history(input);
        free(input);
    }
    return 0;
}
