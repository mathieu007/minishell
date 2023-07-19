#include "minishell.h"
#include <sys/time.h>

int32_t	subshell(char **inputs)
{
	int32_t		ret;
	t_process	*proc;
	char		*str;

	str = *inputs;
	proc = get_process();
	proc->parent_in_fd = -1;
	proc->parent_out_fd = -1;
	proc->is_subshell = true;
	// inputs++;
	// if (*inputs)
	// {
	// 	if (!ft_isnum(*inputs))
	// 		free_all_and_exit2(1, "error: Subshell second arg is not numeric");
	// 	else
	// 		proc->parent_in_fd = ft_atoi(*inputs);
	// 	inputs++;
	// 	if (!ft_isnum(*inputs))
	// 		free_all_and_exit2(1, "error: Subshell third arg is not numeric");
	// 	else
	// 		proc->parent_out_fd = ft_atoi(*inputs);
	// }
	ret = exec_cmds(str);
	free_all_and_exit(proc->errnum);
	return (ret);
}

char	*get_prompt_input(void)
{
	char	*input;

	input = readline("MiniShell> ");
	add_history(input);
	if (input == NULL)
	{
		printf("exit\n");
		free_all_and_exit(0);
	}
	return (input);
}

int32_t	main(int32_t argc, char **argv, char **env)
{
	char	*input;

	init_data(argc, argv, env);
	get_process()->is_subshell = false;
	if (argc >= 2)
		return (subshell(&argv[1]));
	while (1)
	{
		disable_ctrl_c_output();
		setup_signal_handlers();
		input = get_prompt_input();
		exec_cmds(input);
		free(input);
	}
	return (EXIT_SUCCESS);
}
