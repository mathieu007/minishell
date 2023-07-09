#include "minishell.h"
#include <sys/time.h>

int32_t	subshell(char **inputs)
{
	int32_t		ret;
	t_process	*proc;

	proc = get_process();
	ret = exec_cmds(*inputs);
	inputs++;
	while (*inputs)
		ret = exec_cmds(*inputs);
	free_all_and_exit(proc->errnum);
	return (ret);
}

char	*get_prompt_input()
{
	char		*input;

	input = readline("MiniShell> ");
	add_history(input);
	if (input == NULL)
	{
		printf("\x1B[u\x1B[Aexit\n");
		free_all_and_exit(0);
	}	
	return (input);
}

int32_t	main(int32_t argc, char **argv, char **env)
{
	char			*input;

	init_data(argc, argv, env);
	if (argc >= 2)
		return (subshell(&argv[1]));
	disable_ctrl_c_output();
	setup_signal_handlers();
	while (1)
	{	
		// disable_ctrl_c_output();		
		input = get_prompt_input();
		exec_cmds(input);
		free(input);
	}
	return (EXIT_SUCCESS);
}
