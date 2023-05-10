#include "minishell.h"

void	add_env_node(t_data *data, char *variable, char *value)
{
	t_env_cpy	*current;

	current = data->env_cpy;
	while (current->next != NULL)
		current = current->next;
	current->next = create_node(variable, value);
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

void	export_no_variable(t_data *data)
{
	t_env_cpy	*head;
	t_env_cpy	*current;
	bool		swap;

	head = create_list(data);
	current = head;
	while (current)
	{
		if (strcmp(current->variable, current->next->variable) > 0)
		{
			swap_node_value(current, current->next);
			swap = 1;
		}
		if (swap == true)
			current = head;
		else
			current = current->next;
	}
	current = head;
	while(current)
	{
		printf("declare -x %s=\"%s\"\n",current->variable,current->value);
		current= current->next;
	}
	//////FREE chain list
}

void	export_cmd(t_data *data, t_cmd *cmd)
{
	t_env_cpy	*current;
	char		**split_on_equal;
	int			i;
	size_t		len;

	i = 0;
	current = data->env_cpy;
	if (cmd->args[0] == NULL)
		export_no_variable(data);
	if (cmd->options[0] != NULL)
		printf("Export option \"%s\" not handle \n", cmd->options[0]);
	while (cmd->args[i])
	{
		while (current)
		{
			split_on_equal = ft_split(cmd->args[i], '=');
			if (!split_on_equal)
				return ;   ////// ad protection freeee
			len = ft_strlen(split_on_equal[0]);
			if (ft_strnstr(split_on_equal[i], current->variable, len) != 0)
				current->value = cmd->args[i];
			else
				add_env_node(data, split_on_equal[0], split_on_equal[1]);
		}
		i++;
	}
}
