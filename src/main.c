<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 14:40:40 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

=======
>>>>>>> Math
#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	char 	*input;
	t_token *token;
	t_cmd	*cmd;
	t_data 	*data;
	init_data(argc, argv, env);
	char 	*out;

<<<<<<< HEAD
	/////signal handle part /////
	struct sigaction act;
	
    act.sa_sigaction = sig_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
		/////signal handle part /////

	(void)(argc);
	(void)(argv);
	(void)(env);
=======
>>>>>>> Math
	while (1)
	{
		while( signal(SIGINT, sig_handler))
		input = readline("MiniShell> ");
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		out = parse_env(input);
		system(input);
	}
	return (0);
}