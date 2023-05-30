#include "minishell.h"

//bash: export: `1§23': not a valid identifier

void	print_not_valid_identifier(int export_or_unset, char *identifier)
{
	
}

int	is_valid_identifier(char *identifier)
{
	int	res;

	res = 0;
	if (!identifier)
		return (res);
	if (identifier[0])
		if (ft_isalpha(identifier[0]) == 1 || identifier[0] == "_")
			;
	res = 1;
	if (identifier[1])
		if (ft_isalnum(identifier[1] == 1) || identifier[1] == "_"
			|| identifier[1] == "=")
			res = 1;
	return (res);
}

int	unset_cmd(t_cmd *cmd)
{
	t_env_cpy	*current;
	int			i;
	size_t		len;
	char		*var_name;
	t_process	*data;
	t_env_cpy	*temp;

	data = get_process();
	if (cmd->options != NULL)
	{
		printf("Error: Unset option not handled.\n");
		return (1);
	}
	i = 1;
	current = data->env_cpy;
	while (cmd->args[i])
	{
		current = data->env_cpy;
		while (current)
		{
			var_name = ft_strjoin(cmd->args[i], "=");
			len = ft_strlen(var_name);
			if (ft_strncmp(var_name, current->variable, len) == 0)
			{
				if (current->prev)
					current->prev->next = current->next;
				else
					data->env_cpy = current->next;
				if (current->next)
					current->next->prev = current->prev;
				temp = current;
				current = current->next;
				free_t_env_cpy(temp);
			}
			else
			{
				current = current->next;
			}
		}
		i++;
	}
	return (0);
}
