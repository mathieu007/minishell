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

void	export_no_variable(t_process *data)
{
	t_env_cpy	*head;
	t_env_cpy	*current;

	printf("CALLED NO VAR\n");
	head = init_env(data);
	current = head;
	while (current && current->next)
	{
		if (strcmp(current->variable, current->next->variable) > 0)
		{
			swap_node_value(current, current->next);
			current = head;
		}
		else
			current = current->next;
	}
	current = head;
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", current->variable, current->value);
		current = current->next;
	}
	// Free the chain list
	current = head;
	free_t_env_cpy(current);
}

int	export_cmd(t_cmd *cmd)
{
	t_env_cpy *current;
	char **split_on_equal;
	int i;
	size_t len;
	t_process *data;
	bool swap = false;

	data = get_process();
	i = 1;
	current = data->env_cpy;
	if (cmd->args[1] == NULL)
	{
		export_no_variable(data);
		return (0);
	}
	if (cmd->options != NULL)
	{
		printf("Export option \"%s\" not handled\n", cmd->options[0]);
		return (1);
	}
	while (cmd->args[i])
	{
		current = data->env_cpy;
		split_on_equal = ft_split(cmd->args[i], '=');
		if (!split_on_equal)
			return (1); // Add protection for memory allocation failure
		len = ft_strlen(split_on_equal[0]);
		while (current)
		{			
			if (ft_strncmp(split_on_equal[0], current->variable, len) == 0)
			{
				current->value = ft_strdup(split_on_equal[1]);
				swap = true;
				break ; // Exit the loop if the variable is found
			}
			current = current->next;
		}
		if (!swap)
			add_env_node(data, split_on_equal[0], split_on_equal[1]);
		i++;
	}
	free_2d_array((void **)split_on_equal);
	return (0);
}