/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/04/29 09:38:49 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	char	*input;
	(void) (argc);
	(void) (argv);
	(void) (env);

	while (1)
	{
		input = readline("MiniShell> ");
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		system(input);
	}
	return (0);
}