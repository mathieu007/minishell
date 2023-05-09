/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 19:46:05 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_cpy	*create_node(char *variable, char *value)
{
	t_env_cpy	*node;

	node = malloc(sizeof(t_env_cpy));
	if (node == NULL)
		return (NULL);
	return (node);
}

t_env_cpy	*create_list(t_data *data)
{
	char		**split_on_nl;
	char		**split_on_equal;
	t_env_cpy	*head;
	t_env_cpy	*current;
	int			i;

	i = 0;
	split_on_nl = ft_split(data->env, '\n');
	if (!split_on_nl)
		return (NULL);
	split_on_equal = ft_split(split_on_nl[i], '=');
	head = create_node(split_on_equal[0], split_on_equal[1]);
	if (!head)
		return (NULL);
	current = head;
	while (split_on_nl[++i])
	{
		split_on_equal = ft_split(split_on_nl[i], '=');
		current->next = create_command_node(split_on_equal[0],
											split_on_equal[1]);
	if(current->next)
		current->next->prev = current;
		current = current->next;
	}
	return (head);
}

int32_t	env_cmd(t_data *data)
{
	t_env_cpy	*current;

	if (!data->env_cpy)
		create_list(data);
	current = data->env_cpy;
	while (current)
	{
		printf("%s=%s\n", data->env_cpy->variable, data->env_cpy->value);
		current = current->next;
	}
}

//take a variable and return the value
char *get_env_value(char *variable)
{
	t_env_cpy *head;
	t_env_cpy *current;
	size_t len;

	len = ft_strlen(variable);

	head = get_data()->env_cpy;
	current = head;
	while(current)
	{
		if (ft_strnstr(variable, current->value, len) != 0)
			return(current->variable);
		else
		current = current->next;
	}
	return(NULL);
	
}
