#include "minishell.h"

// void	add_env_node(t_data *data, char *variable, char *value)
// {
// 	t_env_cpy	*current;

// 	current = data->env_cpy;
// 	while (current->next != NULL)
// 		current = current->next;
// 	current->next = new_env(variable, value);
// }

// void	swap_node_value(t_env_cpy *a, t_env_cpy *b)
// {
// 	char	*tmp;

// 	tmp = a->value;
// 	a->value = b->value;
// 	b->value = tmp;
// 	tmp = a->variable;
// 	a->variable = b->variable;
// 	b->variable = tmp;
// }

void	export_no_variable(void)
{
	// t_env_cpy	*head;
	// t_env_cpy	*current;
	// bool		swap;

	// head = init_env(data);
	// current = head;
	// while (current)
	// {
	// 	if (strcmp(current->variable, current->next->variable) > 0)
	// 	{
	// 		swap_node_value(current, current->next);
	// 		swap = 1;
	// 	}
	// 	if (swap == true)
	// 		current = head;
	// 	else
	// 		current = current->next;
	// }
	// current = head;
	// while (current)
	// {
	// 	printf("declare -x %s=\"%s\"\n", current->variable, current->value);
	// 	current = current->next;
	// }
	// //////FREE chain list
}

int	export_cmd(t_cmd *cmd)
{
	char		**split_on_equal;
	int			i;
	size_t		len;
	t_data		*data;

	data = get_data();
	i = 0;
	if (cmd->args[0] == NULL)
		export_no_variable();
	if (cmd->options[0] != NULL)
	{
		printf("Export option \"%s\" not handle \n", cmd->options[0]);
		return (1);
	}
	while (cmd->args[i])
	{
		split_on_equal = ft_split(cmd->args[i], '=');
		if (!split_on_equal)
			return(1) ; ////// ad protection freeee
		len = ft_strlen(split_on_equal[0]);	
		while (*environ)
		{			
			if (ft_strnstr(*environ, split_on_equal[0], len) == *environ)
				*environ = cmd->args[i];
			else
				add_env_node(data, split_on_equal[0], split_on_equal[1]);
		}
		i++;
	}
	return(0);
}
