#include "minishell.h"

int	echo_cmd(t_cmd *cmd)
{
	int i;
	int j;

	i = 1;
	while (cmd->args && cmd->args[1] && cmd->args[1][i])
	{
		if (cmd->options)
		{
			if (cmd->args[1][i] != 'n')
			{
				j = 1;
				break ;
			}
			else
				j = 2;
		}
		i++;
	}
	while (cmd->args[j])
	{
		printf("%s", cmd->args[j]);
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