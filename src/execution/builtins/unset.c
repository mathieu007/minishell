#include "minishell.h"

int	unset_cmd(t_cmd *cmd)
{
	t_env_cpy	*current;
	int			i;
	size_t		len;
	t_data		*data;

	data = get_data();
	i = 0;
	current = data->env_cpy;
	if (cmd->options[0] != NULL)
	{
		printf("Unset option \"%s\" not handle \n", cmd->options[0]);
		return(1);
	}
	while (cmd->args[i])
	{
		while (current)
		{
			len = ft_strlen(cmd->args[i]);
			if (ft_strnstr(cmd->args[i], current->variable, len) != 0)
			{
				if (!current->prev)
					data->env_cpy = data->env_cpy->next;
				if (current->prev)
					current->prev->next = current->next;
				if (!current->next)
					current->prev->next = NULL;
			}
			current = current->next;
		}
		i++;
	}
	return(0);
}
