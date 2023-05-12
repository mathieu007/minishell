#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	char 			*input;
	// char 			*out;
	t_token_group	*group;

	init_data(argc, argv, env);
	while (1)
	{
		input = readline("MiniShell> ");
		group = tokenize(input);
		input = parse_env(input);
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		system(input);
	}
	return (0);
}