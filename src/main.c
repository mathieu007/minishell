#include "minishell.h"

int32_t	execute_tests(char *input)
{
	exec_cmds(input);
	// if (strcmp(input, "exit") == 0)
		// break ;
	free_t_token_groups(get_process()->token_groups);
	free_t_cmd(get_process()->cmds);
	return (EXIT_SUCCESS);
}

int32_t	main(int32_t argc, char **argv, char **env)
{
	init_data(argc, argv, env);
	(void)env;
	char 	*input;

	if (argc >= 2)
		return (execute_tests(argv[1]));
	while (1)
	{
		disable_ctrl_c_output();
		setup_signal_handlers();
		input = readline("MiniShell> \x1B[s");
		if(input == NULL)
		{
			printf("\x1B[u\x1B[Aexit\n");
			break;
		}
		exec_cmds(input);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		free_t_token_groups(get_process()->token_groups);
		free_t_cmd(get_process()->cmds);
		free(input);
	}
	return (EXIT_SUCCESS);
}
