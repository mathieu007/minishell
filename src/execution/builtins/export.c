#include "minishell.h"

void	export_no_variable(void)
{
	t_env_cpy	*head;
	t_env_cpy	*current;

	head = copy_env();
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
		if (current->value != NULL)
			printf("declare -x %s=\"%s\"\n", current->variable, current->value);
		else
			printf("declare -x %s", current->variable);
		current = current->next;
	}
	free_t_env_cpy(head);
}

bool	update_env_variable(t_env_cpy *current, char *variable, char *value,
		size_t len)
{
	bool	swap;

	swap = false;
	while (current)
	{
		if (ft_strncmp(variable, current->variable, len) == 0)
		{
			current->value = ft_strdup(value);
			swap = true;
			break ;
		}
		current = current->next;
	}
	return (swap);
}

char	*handle_export_value(char **split_on_equal)
{
	char	*value;

	value = ft_strdup(split_on_equal[1]);
	if (count_splits(split_on_equal) > 2)
		value = join_splits(&split_on_equal[1], "=");
	return (value);
}

void	handle_export(t_process *data, char *arg)
{
	t_env_cpy	*current;
	char		**split_on_equal;
	char		*value;
	size_t		len;
	bool		swap;

	current = data->env_cpy;
	split_on_equal = ft_split(arg, '=');
	if (!split_on_equal)
		return ;
	value = handle_export_value(split_on_equal);
	if (is_valid_identifier(split_on_equal[0]) == 0)
	{
		print_not_valid_identifier(0, split_on_equal[0]);
		free_2d_array((void **)split_on_equal);
		return ;
	}
	len = ft_strlen(split_on_equal[0]);
	swap = update_env_variable(current, split_on_equal[0], value, len);
	if (!swap)
		add_env_node(data, split_on_equal[0], value);
	free_2d_array((void **)split_on_equal);
}

int	export_cmd(t_cmd *cmd)
{
	t_process	*data;
	int			i;

	data = get_process();
	if (cmd->args[1] == NULL)
	{
		export_no_variable();
		return (0);
	}
	if (cmd->options != NULL)
	{
		printf("Export option \"%s\" not handled\n", cmd->options[0]);
		return (1);
	}
	i = 1;
	while (cmd->args[i])
	{
		handle_export(data, cmd->args[i]);
		i++;
	}
	return (0);
}

// int	export_cmd(t_cmd *cmd)
// {
// 	t_env_cpy *current;
// 	char **split_on_equal;
// 	int i;
// 	size_t len;
// 	t_process *data;
// 	bool swap = false;
// 	char *value;

// 	data = get_process();
// 	i = 1;
// 	current = data->env_cpy;
// 	if (cmd->args[1] == NULL)
// 	{
// 		export_no_variable();
// 		return (0);
// 	}
// 	if (cmd->options != NULL)
// 	{
// 		printf("Export option \"%s\" not handled\n", cmd->options[0]);
// 		return (1);
// 	}
// 	while (cmd->args[i])
// 	{
// 		current = data->env_cpy;
// 		split_on_equal = ft_split(cmd->args[i], '=');
// 		if (!split_on_equal)
// 			return (1);
// 		value = ft_strdup(split_on_equal[1]);
// 		if (count_splits(split_on_equal) > 2)
// 			value = join_splits(&split_on_equal[1], "=");

// 		if (is_valid_identifier(split_on_equal[0]) == 0)
// 			print_not_valid_identifier(0, split_on_equal[0]);
// 		len = ft_strlen(split_on_equal[0]);
// 		while (current)
// 		{
// 			if (ft_strncmp(split_on_equal[0], current->variable, len) == 0)
// 			{
// 				current->value = ft_strdup(value);
// 				swap = true;
// 				break ;
// 			}
// 			current = current->next;
// 		}
// 		if (!swap)
// 			add_env_node(data, split_on_equal[0], value);
// 		i++;
// 	}
// 	free_2d_array((void **)split_on_equal);
// 	return (0);
// }