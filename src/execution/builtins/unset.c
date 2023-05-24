#include "minishell.h"

int	unset_cmd(t_cmd *cmd)
{
	t_env_cpy	*current;
	int			i;
	size_t		len;
	t_process	*data;
	t_env_cpy	*temp;

	data = get_process();
	if (cmd->options != NULL)
	{
		printf("Error: Unset option not handled.\n");
		return (1);
	}
	i = 1;
	current = data->env_cpy;
	while (cmd->args[i])
	{
		current = data->env_cpy;
		while (current)
		{
			len = ft_strlen(cmd->args[i]);
			if (ft_strncmp(cmd->args[i], current->variable, len) == 0)
			{
				if (current->prev)
					current->prev->next = current->next;
				else
					data->env_cpy = current->next;
				if (current->next)
					current->next->prev = current->prev;
				temp = current;
				current = current->next;
				free_t_env_cpy(temp);
			}
			else
			{
				current = current->next;
			}
		}
		i++;
	}
	printf("\n");
	return (0);
}
