/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/26 08:19:10 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_no_variable(void)
{
	t_env_cpy	*head;
	t_env_cpy	*cur;

	head = copy_env();
	cur = head;
	while (cur && cur->next)
	{
		if (strcmp(cur->variable, cur->next->variable) > 0)
		{
			swap_node_value(cur, cur->next);
			cur = head;
		}
		else
			cur = cur->next;
	}
	cur = head;
	while (cur)
	{
		if (cur->value != NULL)
			ft_printf("declare -x %s=\"%s\"\n", cur->variable, cur->value);
		else
			ft_printf("declare -x %s", cur->variable);
		cur = cur->next;
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
		name = arg;
	if (value && value[0])
		value = ft_strdup(&value[1]);
	if (name && is_valid_identifier(name) == 0)
		return (print_not_valid_identifier(0, name), 1);
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
