#include "minishell.h"

void	env_cmd(t_data *data)
{
	t_env_cpy	*current;

	if (!data->env_cpy)
		data->env_cpy = init_env(data);
	current = data->env_cpy;
	while (current)
	{
		printf("%s=%s\n", data->env_cpy->variable, data->env_cpy->value);
		current = current->next;
	}
}
