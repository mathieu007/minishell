#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	char 		*input;
	char		**args;
	char		**envp;
	// char 			*out;
	t_token_group	*group;
	args = malloc(4 * sizeof(char *));
	envp = parse_env_path(env);

	args[0] = "echo";
	args[1] = "123";
	args[2] = "123";
	args[3] = NULL;
	init_data(argc, argv, env);
	if (execve("/bin/echo", args, envp) == -1)
		perror("Could not execve");
	return (1);
	while (1)
	{
		input = readline("MiniShell> ");
		group = tokenize(input);
		if (!group)
			group = NULL;
		// input = parse_env(input);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		system(input);
	}
	return (0);
}
