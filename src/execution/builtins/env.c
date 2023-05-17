#include "minishell.h"

t_env_cpy	*new_env(char *variable, char *value)
{
	t_env_cpy	*node;

	node = malloc(sizeof(t_env_cpy));
	if (node == NULL)
		return (NULL);
	node->value = value;
	node->variable = variable;
	return (node);
}

t_env_cpy	*init_env(t_data *data)
{
	char		**split_on_equal;
	t_env_cpy	*head;
	t_env_cpy	*current;
	int			i;

	if (!data->env)
		return (NULL);
	i = 0;
	split_on_equal = ft_split(data->env[i], '=');
	current = new_env(split_on_equal[0], split_on_equal[1]);
	i++;
	head = current;
	while (data->env[i])
	{		
		split_on_equal = ft_split(data->env[i], '=');
		current->next = new_env(split_on_equal[0], split_on_equal[1]);
		if (current->next)
			current->next->prev = current;
		current = current->next;
		i++;
	}
	return (head);
}

//take a variable and return the value
char	*get_env_value(char *variable)
{
	t_env_cpy	*head;
	t_env_cpy	*current;
	size_t		len;

	len = ft_strlen(variable);
	head = get_data()->env_cpy;
	current = head;
	while (current && current->value)
	{
		if (ft_strnstr(current->variable, variable, len) == current->variable)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}