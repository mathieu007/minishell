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

int32_t	count_splits(char **split)
{
	int32_t	i;

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
	size_t	total_length;
	char	*str;
	char	**temp;

	total_length = 0;
	temp = split;
	while (*temp != NULL)
	{
		total_length += ft_strlen(*temp);
		if (*(temp + 1) != NULL)
			total_length += ft_strlen(join);
		temp++;
	}
	str = (char *)malloc(sizeof(char) * (total_length + 1));
	if (str == NULL)
		return (NULL);
	str[0] = '\0';
	while (*split != NULL)
	{
		ft_strcat(str, *split);
		if (*(split + 1) != NULL)
			ft_strcat(str, join);
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
	char		*tmp_str;

	if (!data->env)
		return (NULL);
	i = 0;
	split_on_equal = ft_split(data->env[i], '=');
	count = count_splits(split_on_equal);
	if (count > 2)
		current = new_env(ft_strdup(split_on_equal[0]),
							join_splits(&split_on_equal[1], "="));
	else
		current = new_env(ft_strdup(split_on_equal[0]),
							ft_strdup(split_on_equal[1]));
	if (split_on_equal)
		free_2d_char_array(split_on_equal);
	i++;
	head = current;
	while (data->env[i])
	{
		split_on_equal = ft_split(data->env[i], '=');
		count = count_splits(split_on_equal);
		tmp_str = join_splits(&split_on_equal[1], "=");
		if (count > 2)
			current->next = new_env(ft_strdup(split_on_equal[0]),
									ft_strdup(tmp_str));
		else
			current->next = new_env(ft_strdup(split_on_equal[0]),
									ft_strdup(split_on_equal[1]));
		if (tmp_str)
			free(tmp_str);
		if (current->next)
			current->next->prev = current;
		if (split_on_equal)
			free_2d_char_array(split_on_equal);
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
	printf("current-adress %lu \n", (uintptr_t)current);
	printf("current-var = %s\n", current->variable);
	printf("var = %s\n", variable);
	printf("len = %zu\n", len);
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
	t_env_cpy *head;
	t_env_cpy *current;
	t_env_cpy *new_head;
	t_env_cpy *new_current;

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