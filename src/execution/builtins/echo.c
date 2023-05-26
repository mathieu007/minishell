#include "minishell.h"

int	echo_cmd(t_cmd *cmd)
{
	int i;
	int j;
	
	
	// dangerous initialize j:
	// you should verify if there is a character that is not an "n", 
	// if so print arg1 arg2 arg3... as is
	// not what's happening here
	j = 2;
	i = 1;
	while (cmd->args && cmd->args[1] && cmd->args[1][i])
	{
		if (cmd->args[1][i] != 'n')
			j = 1;
		i++;
	}
	while (cmd->args[j])
	{
		printf("%s", cmd->args[j]);
		if (cmd->args[j + 1])
			printf(" ");
		j++;
	}
	if (cmd->options != NULL)
		if ((ft_strnstr(cmd->options[0], "-n",
					ft_strlen(cmd->options[0]))) != 0)
			return (0);

	printf("\n");
	return (0);
}