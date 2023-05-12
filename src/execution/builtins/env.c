#include "minishell.h"

int	env_cmd(t_cmd *cmd)

#include "minishell.h"

t_env_cpy	*create_node(char *variable, char *value)
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

	i = 0;
	while(data->env[i])
	split_on_equal = ft_split(split_on_nl[i], '=');
	head = create_node(split_on_equal[0], split_on_equal[1]);
	if (!head)
		return (NULL);
	current = head;
	while (split_on_nl[++i])
	{
		split_on_equal = ft_split(split_on_nl[i], '=');
		current->next = create_node(split_on_equal[0],
											split_on_equal[1]);
	if(current->next)
		current->next->prev = current;
	if (!data->env)
		return (NULL);
	i = 0;
	split_on_equal = ft_split(data->env[i], '=');
	current = create_node(split_on_equal[0], split_on_equal[1]);
	i++;
	head = current;
	while (data->env[i])
	{
		split_on_equal = ft_split(data->env[i], '=');
		current->next = create_node(split_on_equal[0], split_on_equal[1]);
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
	t_data		*data;

	(void)cmd;
	data = get_data();
	if (!data->env_cpy)
		data->env_cpy = init_env(data);
	current = data->env_cpy;
	while (current)
	{
		printf("%s=%s\n", data->env_cpy->variable, data->env_cpy->value);
		current = current->next;
	}
	return(0);
}
