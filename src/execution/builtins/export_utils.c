#include "minishell.h"

void	add_env_node(t_process *data, char *variable, char *value)
{
	t_env_cpy	*current;

	current = data->env_cpy;
	while (current->next != NULL)
		current = current->next;
	current->next = new_env(ft_strdup(variable), ft_strdup(value));
}

void	swap_node_value(t_env_cpy *a, t_env_cpy *b)
{
	char	*tmp;

	tmp = a->value;
	a->value = b->value;
	b->value = tmp;
	tmp = a->variable;
	a->variable = b->variable;
	b->variable = tmp;
}
