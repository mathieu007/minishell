#include "minishell.h"

void	print_not_valid_identifier(int export_or_unset, char *identifier)
{
	int	i;

	i = -1;
	if (export_or_unset == 0)
	{
		write(2, "Minishell: export: `", 20);
		while (identifier[++i])
			write(2, &identifier[i], 1);
		write(2, ": not a valid identifier\n", 25);
	}
	if (export_or_unset == 1)
	{
		write(2, "Minishell: unset: `", 19);
		while (identifier[++i])
			write(2, &identifier[i], 1);
		write(2, ": not a valid identifier\n", 25);
	}
}

int	is_valid_identifier(char *identifier)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	if (!identifier)
		return (res);
	while (identifier[i])
	{
		if (identifier[0])
			if (ft_isalpha(identifier[0]) == 1 || identifier[0] == '_')
				res = 1;
		if (ft_isalnum(identifier[i]) != 1 && identifier[i] != '_'
			&& identifier[i] != '=' && identifier[i] != '\0')
			res = 0;
		i++;
	}
	return (res);
}

int	is_valid_identifier_unset(char *identifier)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	if (!identifier)
		return (res);
	while (identifier[i])
	{
		if (identifier[0])
			if (ft_isalpha(identifier[0]) == 1 || identifier[0] == '_')
				res = 1;
		if (ft_isalnum(identifier[i]) != 1 && identifier[i] != '_'
			&& identifier[i] != '\0')
			res = 0;
		if (identifier[i] == '=')
		{
			res = 0;
			break ;
		}
		i++;
	}
	return (res);
}

int	unset_cmd(t_cmd *cmd)
{
	t_env_cpy *current;
	int i;
	size_t len;
	t_process *data;

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
			i++; // Increment i to continue with the next argument
			continue ;
		}

		current = data->env_cpy;
		t_env_cpy *prev = NULL; // Keep track of the previous node
		while (current)
		{
			len = ft_strlen(cmd->args[i]);
			if (ft_strncmp(cmd->args[i], current->variable, len) == 0)
			{
				if (prev)
					prev->next = current->next;
				else
					data->env_cpy = current->next; // Update head of the list

				if (current->next)
					current->next->prev = prev;

				t_env_cpy *temp = current;
				current = current->next;
				free(temp); // Free the memory of the removed node

				break ; // Exit the inner loop after removal
			}

			prev = current;
			current = current->next;
		}

		i++;
	}

	return (0);
}