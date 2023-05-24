#include "minishell.h"

int	echo_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->options)
	{	
		if ((ft_strnstr(cmd->options[i], "-n", ft_strlen(cmd->options[i]))) != 0)
		{
			while ((ft_strnstr(cmd->options[i], "-n",
						ft_strlen(cmd->options[i]))) != 0)
				i++;
		}
		i = 0;
		if (cmd->options[0])
			i++;
	}	
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		i++;
	}
	if (cmd->options && (!ft_strnstr(cmd->options[0], "-n", ft_strlen(cmd->options[0]))) != 0)
		printf("\n");
	return (0);
	printf("\n");
}
