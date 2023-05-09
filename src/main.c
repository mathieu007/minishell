/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 13:19:24 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	char	*input;
	t_cmd	*cmd;
	t_data *data;

	(void)(argc);
	(void)(argv);
	(void)(env);
	while (1)
	{
		input = readline("MiniShell> ");
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		system(input);
		if (*input != '\0')
		{
			cmd = get_first_cmd();
			data = (get_data);
			//execute avec execve
			if (!cmd->is_builtin)
				printf("NEED TO EXCVE THIS INPUT\n");
			//execute avec built in
			else
				execute_built_in(cmd , data);
		}
		//FREEEEE ALLL
	}
}
