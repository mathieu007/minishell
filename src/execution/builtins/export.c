/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/06/26 15:41:18 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			current->value = free_ptr(current->value);
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

	value = (split_on_equal[1]);
	if (count_splits(split_on_equal) > 2)
		value = join_splits(&split_on_equal[1], "=");
	return (value);
}

int32_t	handle_export(t_process *data, char *arg)
{
	t_env_cpy	*current;
	char		**split_on_equal;
	char		*value;
	size_t		len;
	bool		swap;

	current = data->env_cpy;
	split_on_equal = ft_split(arg, '=');
	if (!split_on_equal)
		return (0);
	value = handle_export_value(split_on_equal);
	if (is_valid_identifier(split_on_equal[0]) == 0)
	{
		print_not_valid_identifier(0, split_on_equal[0]);
		free_2d_array((void **)split_on_equal);
		return (1);
	}
	len = ft_strlen(split_on_equal[0]);
	swap = update_env_variable(current, split_on_equal[0], value, len);
	if (!swap)
		add_env_node(data, split_on_equal[0], value);
	free_2d_array((void **)split_on_equal);
	return (0);
}

int	export_cmd(t_cmd *cmd)
{
	t_process	*data;
	int			i;
	char		*arg;
	int			ret;

	ret = 0;
	data = get_process();
	if (cmd->args[1] == NULL)
		return (export_no_variable(), 0);
	if (cmd->options != NULL)
	{
		printf("Export option \"%s\" not handled\n", cmd->options[0]);
		return (1);
	}
	i = 1;
	while (cmd->args[i])
	{
		arg = cmd->args[i];
		if (handle_export(data, arg) > 0)
			ret = 1;
		i++;
	}
	return (ret);
}
