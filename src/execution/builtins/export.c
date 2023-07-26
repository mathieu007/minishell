/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/26 07:43:46 by math             ###   ########.fr       */
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
			ft_printf("declare -x %s=\"%s\"\n", current->variable,
					current->value);
		else
			ft_printf("declare -x %s", current->variable);
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
	char		*value;
	size_t		len;
	bool		swap;
	char		*name;

	current = data->env_cpy;
	value = ft_strchr(arg, '=');
	if (value)
		name = ft_substr(arg, 0, value - arg);
	else
		name = value;
	if (value && value[0])
		value = ft_strdup(&value[1]);
	if (is_valid_identifier(name) == 0)
	{
		print_not_valid_identifier(0, name);
		return (1);
	}
	if (!value)
		return (0);
	len = ft_strlen(name);
	swap = update_env_variable(current, name, value, len);
	if (!swap)
		add_env_node(data, name, value);
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
		ft_printf("Export option \"%s\" not handled\n", cmd->options[0]);
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
