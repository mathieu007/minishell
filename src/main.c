#include "minishell.h"
#include <sys/time.h>

int32_t	execute_tests(char *input)
{
	int32_t	ret;

	ret = exec_cmds(input);
	free_t_data(get_process());
	return (ret);
}

int32_t	main(int32_t argc, char **argv, char **env)
{
	init_data(argc, argv, env);
	(void)env;
	char 	*input;

	if (argc >= 2)
	{
		return (execute_tests(argv[1]));
	}
	while (1)
	{
		input = readline("MiniShell> ");
		exec_cmds(input);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		free_t_data(get_process());
		free(input);
	}
	return (EXIT_SUCCESS);
}
