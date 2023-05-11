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

t_env_cpy	*create_list(t_data *data)
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

void	env_cmd(t_data *data)
{
	t_env_cpy	*current;

	if (!data->env_cpy)
		data->env_cpy = create_list(data);
	current = data->env_cpy;
	while (current)
	{
		printf("%s=%s\n", data->env_cpy->variable, data->env_cpy->value);
		current = current->next;
	}
}

//take a variable and return the value
char	*get_env_value(char *variable)
{
	#ifdef _DEBUG
		printf("get_env_value\n");
	#endif
	t_env_cpy	*head;
	t_env_cpy	*current;
	size_t		len;

	len = ft_strlen(variable);
	head = get_data()->env_cpy;
	current = head;
	while (current && current->value)
	{
		#ifdef _DEBUG
			printf("	var:%s; value:%s;\n", current->variable, current->value);
		#endif
		if (ft_strnstr(variable, current->variable, len) != 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
