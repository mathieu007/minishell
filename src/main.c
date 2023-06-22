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


int32_t	main(int32_t argc, char **argv, char **env)
{
	char			*input;
	t_process		*proc;

	proc = get_process();
	proc->program = &ft_strchrlast(argv[0], '/')[1];
	proc->full_program_name = argv[0];
	init_data(argc, argv, env);
	(void)env;
	if (argc >= 2)
		return (subshell(&argv[1]));
	while (1)
	{
		disable_ctrl_c_output();
		setup_signal_handlers();
		input = readline("MiniShell> ");
		if (input == NULL)
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
	}
	return (EXIT_SUCCESS);
}
