#include "minishell.h"

extern char	**environ;

int32_t	main(int32_t argc, char **argv, char **env)
{
	(void)env;
	char 		*input;
	// char		**args;
	// char		**envp;
	// t_data			*data;

	// data = get_data();
	init_data(argc, argv);
	// args = malloc(4 * sizeof(char *));	
	// envp = parse_env_path(get_data()->env_cpy);
	// args[0] = "echo";
	// args[1] = "$VAR";
	// args[2] = NULL;
	// args[3] = NULL;
	
	// print_env(data->env_cpy);
	system("export VAR=123");
	// if (execve("/usr/bin/echo", args, envp) == -1)
	// 	perror("Could not execve");
	while (1)
	{
		input = readline("MiniShell> ");
		exec_cmds(input);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		free_all();
	}
}
