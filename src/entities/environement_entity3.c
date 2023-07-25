/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_entity3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/24 13:09:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env_cpy	*create_env_node(char *key, char *value)
{
	t_env_cpy	*node;

	if (!key)
		return (NULL);
	node = new_env(key, value);
	return (node);
}

static void	free_split_on_equal(char **split_on_equal)
{
	free_2d_char_array(split_on_equal);
}

static void	process_env_node(t_env_cpy **head, t_env_cpy **current,
		char **split_on_equal)
{
	int32_t		count;
	char		*tmp_str;
	t_env_cpy	*node;

	count = count_splits(split_on_equal);
	tmp_str = join_splits(&split_on_equal[1], "=");
	node = create_env_node(split_on_equal[0], tmp_str);
	free(tmp_str);
	if (!node)
		return (free_split_on_equal(split_on_equal));
	if (!(*head))
		*head = node;
	else
		(*current)->next = node;
	node->prev = *current;
	*current = node;
	free_split_on_equal(split_on_equal);
}

t_env_cpy	*init_env(t_process *data)
{
	char		**split_on_equal;
	t_env_cpy	*head;
	t_env_cpy	*current;
	int			i;

	if (!data || !data->env)
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (data->env[i])
	{
		split_on_equal = ft_split(data->env[i], '=');
		if (!split_on_equal)
			return (free_2d_char_array(split_on_equal), NULL);
		process_env_node(&head, &current, split_on_equal);
		i++;
	}
	return (head);
}

// t_env_cpy	*init_env(t_process *data)
// {
// 	char		**split_on_equal;
// 	t_env_cpy	*head;
// 	t_env_cpy	*current;
// 	int			i;
// 	int32_t		count;
// 	char		*tmp_str;

// 	if (!data->env)
// 		return (NULL);
// 	i = 0;
// 	split_on_equal = ft_split(data->env[i], '=');
// 	count = count_splits(split_on_equal);
// 	if (count > 2)
// 	{
// 		tmp_str = join_splits(&split_on_equal[1], "=");
// 		current = new_env(split_on_equal[0], tmp_str);
// 		if (tmp_str)
// 			free(tmp_str);
// 	}
// 	else
// 		current = new_env(split_on_equal[0], split_on_equal[1]);
// 	if (split_on_equal)
// 		free_2d_char_array(split_on_equal);
// 	i++;
// 	head = current;
// 	while (data->env[i])
// 	{
// 		split_on_equal = ft_split(data->env[i], '=');
// 		count = count_splits(split_on_equal);
// 		tmp_str = join_splits(&split_on_equal[1], "=");
// 		if (count > 2)
// 			current->next = new_env(split_on_equal[0], tmp_str);
// 		else
// 			current->next = new_env(split_on_equal[0], split_on_equal[1]);
// 		if (tmp_str)
// 			free(tmp_str);
// 		if (current->next)
// 			current->next->prev = current;
// 		if (split_on_equal)
// 			free_2d_char_array(split_on_equal);
// 		current = current->next;
// 		i++;
// 	}
// 	return (head);
// }
