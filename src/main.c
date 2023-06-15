#include "minishell.h"
#include <sys/time.h>

int32_t	execute_tests(char *input)
{
	int32_t	ret;

	ret = exec_cmds(input);
	free_t_process(get_process());
	return (ret);
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
		input = readline("MiniShell> ");
		if(input == NULL)
		{
			printf("\x1B[u\x1B[Aexit\n");
		free_all_and_exit(0);
			break;
		}
		exec_cmds(input);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		free(input);
		//free_all_and_exit(0);
	}
	return (EXIT_SUCCESS);
}
