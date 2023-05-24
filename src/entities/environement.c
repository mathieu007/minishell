#include "minishell.h"

t_env_cpy	*new_env(char *variable, char *value)
{
	t_env_cpy	*node;

	node = ft_calloc(1, sizeof(t_env_cpy));
	if (node == NULL)
		return (NULL);
	node->value = value;
	node->variable = variable;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

t_env_cpy	*init_env(t_process *data)
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
	head = get_process()->env_cpy;
	current = head;
	while (current)
	{
		if (ft_strnstr(current->variable, variable, len) == current->variable)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

//make a copy of the environement variable
t_env_cpy	*copy_env(void)
{
	t_env_cpy	*head;
	t_env_cpy	*current;
	t_env_cpy	*new_head;
	t_env_cpy	*new_current;

	head = get_process()->env_cpy;
	current = head;
	if (current)
	{
		new_current = new_env(ft_strdup(current->variable),
				ft_strdup(current->value));
		new_head = new_current;
		while (current)
		{
			new_current->next = new_env(ft_strdup(current->variable),
					ft_strdup(current->value));
			new_current->next->prev = new_current;
			new_current = new_current->next;
			current = current->next;
		}
		return (new_head);
	}
	return (NULL);
}