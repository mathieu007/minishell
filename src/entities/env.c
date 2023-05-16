#include "minishell.h"

int32_t	env_cmd(t_cmd *cmd)
{
	(void)cmd;
	t_data 		*data;
	t_env_cpy	*current;

	data = get_data();
	if (!data->env_cpy)
		data->env_cpy = init_env(data);
	current = data->env_cpy;
	while (current)
	{
		printf("%s=%s\n", data->env_cpy->variable, data->env_cpy->value);
		current = current->next;
	}
	return (1);
}
