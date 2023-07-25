/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_entity2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/24 13:09:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	count_env(void)
{
	t_env_cpy	*head;
	t_env_cpy	*current;
	int32_t		count;

	count = 0;
	head = get_process()->env_cpy;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	**get_env(void)
{
	t_env_cpy	*head;
	t_env_cpy	*current;
	int32_t		count;
	char		**data;
	int32_t		i;

	i = 0;
	count = count_env();
	data = malloc(sizeof(char *) * (count + 1));
	if (!data)
		free_all_and_exit2(errno, "malloc error");
	data[count] = NULL;
	head = get_process()->env_cpy;
	current = head;
	while (current)
	{
		data[i] = ft_strjoin(current->variable, "=");
		data[i] = ft_strjoinfree(data[i], current->value);
		current = current->next;
		i++;
	}
	return (data);
}

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
		if (ft_strequal(current->variable, variable) 
			&& len == ft_strlen(current->variable))
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}

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
		new_current = new_env(current->variable, current->value);
		new_head = new_current;
		while (current)
		{
			new_current->next = new_env(current->variable, current->value);
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
		new_current = new_env(current->variable, current->value);
		new_head = new_current;
		while (current)
		{
			new_current->next = new_env(current->variable, current->value);
			new_current->next->prev = new_current;
			new_current = new_current->next;
			current = current->next;
		}
		return (new_head);
	}
	return (NULL);
}
