#include "minishell.h"

int	env_cmd(t_cmd *cmd)
{
	t_env_cpy	*current;
	t_data		*data;

	(void)cmd;
	data = get_data();
	if (!data->env_cpy)
		data->env_cpy = init_env(data);
	current = data->env_cpy;
	while (current)
	{
		printf("%s=%s\n", data->env_cpy->variable, data->env_cpy->value);
		current = current->next;
	}
	return(0);
}
