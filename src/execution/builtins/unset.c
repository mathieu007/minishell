#include "minishell.h"

void	remove_node_from_list(t_process *data, t_env_cpy *node, t_env_cpy *prev)
{
	if (prev)
		prev->next = node->next;
	else
		data->env_cpy = node->next;
	if (node->next)
		node->next->prev = prev;
	free(node);
}

void	remove_env_var(t_process *data, char *variable)
{
	t_env_cpy	*current;
	t_env_cpy	*prev;
	size_t		len;

	current = data->env_cpy;
	prev = NULL;
	while (current)
	{
		len = ft_strlen(variable);
		if (ft_strncmp(variable, current->variable, len) == 0)
		{
			remove_node_from_list(data, current, prev);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

int	unset_cmd(t_cmd *cmd)
{
	t_process	*data;
	int			i;

	data = get_process();
	if (cmd->options != NULL)
	{
		printf("Error: Unset option not handled.\n");
		return (1);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (is_valid_identifier_unset(cmd->args[i]) == 0)
		{
			print_not_valid_identifier(1, cmd->args[i]);
			i++;
			continue ;
		}
		remove_env_var(data, cmd->args[i]);
		i++;
	}
	return (0);
}

// int	unset_cmd(t_cmd *cmd)
// {
// 	t_env_cpy *current;
// 	int i;
// 	size_t len;
// 	t_process *data;
// 	t_env_cpy *prev;

// 	data = get_process();
// 	if (cmd->options != NULL)
// 	{
// 		printf("Error: Unset option not handled.\n");
// 		return (1);
// 	}

// 	i = 1;
// 	while (cmd->args[i])
// 	{
// 		if (is_valid_identifier_unset(cmd->args[i]) == 0)
// 		{
// 			print_not_valid_identifier(1, cmd->args[i]);
// 			i++; // Increment i to continue with the next argument
// 			continue ;
// 		}

// 		current = data->env_cpy;
// 		prev = NULL; // Keep track of the previous node
// 		while (current)
// 		{
// 			len = ft_strlen(cmd->args[i]);
// 			if (ft_strncmp(cmd->args[i], current->variable, len) == 0)
// 			{
// 				if (prev)
// 					prev->next = current->next;
// 				else
// 					data->env_cpy = current->next; // Update head of the list

// 				if (current->next)
// 					current->next->prev = prev;

// 				t_env_cpy *temp = current;
// 				current = current->next;
// 				free(temp); // Free the memory of the removed node

// 				break ; // Exit the inner loop after removal
// 			}

// 			prev = current;
// 			current = current->next;
// 		}

// 		i++;
// 	}

// 	return (0);
// }