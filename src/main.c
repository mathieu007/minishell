#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	char 		*input;
	char		**args;
	char		**envp;
	t_data			*data;

	data = get_data();
	args = malloc(4 * sizeof(char *));	
	envp = parse_env_path(env);
	args[0] = "echo";
	args[1] = "$VAR";
	args[2] = NULL;
	args[3] = NULL;
	init_data(argc, argv, env);	
	// print_env(data->env_cpy);
	system("export VAR=123");
	if (execve("/usr/bin/echo", args, envp) == -1)
		perror("Could not execve");
	while (1)
	{
		input = readline("MiniShell> ");
		system(input);
		tokenize_groups(input);
		// tokenize(input);
		//get_seq_cmds(data->token_groups);
		print_groups_and_tokens();
		print_cmd(data->cmds);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		free_all();
	}
}
