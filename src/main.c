#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	char 		*input;
	char		**args;
	char		**envp;
	t_data		*data;
	// char 			*out;

	data = get_data();
	args = malloc(4 * sizeof(char *));
	envp = parse_env_path(env);

	args[0] = "echo";
	args[1] = "123";
	args[2] = "123";
	args[3] = NULL;
	init_data(argc, argv, env);
	// if (execve("/bin/echo", args, envp) == -1)
	// 	perror("Could not execve");
	while (1)
	{
		input = readline("MiniShell> ");
		data->token_groups = tokenize(input);
		data->tokens = data->token_groups->first_token;
		print_token_group(data->token_groups);
		print_token(data->token_groups->first_token);
		// input = parse_env(input);
		// input = parse_env(input);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		free_all();
	}
}
