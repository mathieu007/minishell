/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */


#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	init_data(argc, argv, env);
	char *input;
	t_token *token;
	char	*input;
	t_cmd	*cmd;
	t_data *data;

	while (1)
	{
		input = readline("MiniShell> ");
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		system(input);
		if (*input != '\0')
		{
		// token = tokenize(input);
		// parse_cmds(token);
			cmd = get_first_cmd();
			data = (get_data);
			//execute avec execve
			if (!cmd->is_builtin)
				printf("NEED TO EXCVE THIS INPUT\n");
			else
				execute_built_in(cmd , data);
		}
		//FREEEEE ALLL
	}
	return (0);
}