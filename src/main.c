#include "minishell.h"

void execute_cmd(char *input)
{
	exec_cmds(input);
	// if (strcmp(input, "exit") == 0)
		// break ;
	add_history(input);
	free_t_token_groups(get_process()->token_groups);
	free_t_cmd(get_process()->cmds);
}

int32_t	main(int32_t argc, char **argv, char **env)
{
	init_data(argc, argv, env);
	(void)env;
	char 		*input;

	while (1)
	{
		input = readline("MiniShell> ");
		exec_cmds(input);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		free_t_token_groups(get_process()->token_groups);
		free_t_cmd(get_process()->cmds);
	}
}
