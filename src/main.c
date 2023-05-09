/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 14:01:32 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	init_data(argc, argv, env);
	char *input;
	t_token *token;

	while (1)
	{
		input = readline("MiniShell> ");
		if (strcmp(input, "exit") == 0)
			break ;
		token = tokenize(input);
		parse_cmds(token);
		add_history(input);
		system(input);
	}
	return (0);
}