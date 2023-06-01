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

int32_t		count_splits(char **split)
{
	int32_t i;
	
	i = 0;
	while (*split)
	{
		split++;
		i++;
	}
	return (i);
}

char	*join_splits(char **split, char *join)
{
	char	*str;

	str = *split;
	if (split[1])
		str = ft_strjoinfree(str, join);
	split++;
	while (*split)
	{
		str = ft_strjoinfree(str, *split);
		if (!str)
			return (NULL);
		if (split[1])
			str = ft_strjoinfree(str, join);
		if (!str)
			return (NULL);
		split++;
	}
	return (str);
}

t_env_cpy	*init_env(t_process *data)
{
	char		**split_on_equal;
	t_env_cpy	*head;
	t_env_cpy	*current;
	int			i;
	int32_t		count;

	if (!data->env)
		return (NULL);
	i = 0;
	split_on_equal = ft_split(data->env[i], '=');
	count = count_splits(split_on_equal);
	if (count > 2)
		current = new_env(split_on_equal[0], join_splits(&split_on_equal[1], "="));
	else
		current = new_env(split_on_equal[0], split_on_equal[1]);
	i++;
	head = current;
	while (data->env[i])
	{		
		split_on_equal = ft_split(data->env[i], '=');
		count = count_splits(split_on_equal);
		if (count > 2)
			current->next = new_env(split_on_equal[0], join_splits(&split_on_equal[1], "="));
		else	
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
			return (ft_strdup(current->value));
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

t_env_cpy	*copy_env_from(t_process *proc)
{
	t_env_cpy	*head;
	t_env_cpy	*current;
	t_env_cpy	*new_head;
	t_env_cpy	*new_current;

	head = proc->env_cpy;
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
