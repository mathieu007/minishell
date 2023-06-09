#include "minishell.h"

int32_t	env_cmd(t_cmd *cmd)
{
	(void)cmd;
	t_process *data;
	t_env_cpy *current;

	data = get_process();
	if (!data->env_cpy)
		data->env_cpy = init_env(data);
	current = data->env_cpy;
	while (current && current->value != NULL)
	{
		printf("%s=%s\n", current->variable, current->value);
		current = current->next;
	}
	return (1);
}