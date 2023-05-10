#include "minishell.h"

int32_t	main(int32_t argc, char **argv, char **env)
{
	char 	*input;
	t_token *token;
	t_cmd	*cmd;
	t_data 	*data;
	init_data(argc, argv, env);
	char 	*out;

	while (1)
	{
		input = readline("MiniShell> ");
		if (strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		out = parse_env(input);
		system(input);
	}
	return (0);
}