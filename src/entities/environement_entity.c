/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement_entities.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/06/26 15:41:18 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_cpy	*new_env(char *variable, char *value)
{
	t_env_cpy	*node;

	node = ft_calloc(1, sizeof(t_env_cpy));
	if (node == NULL)
		return (NULL);
	node->value = ft_strdup(value);
	node->variable = ft_strdup(variable);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

int32_t	count_splits(char **split)
{
	int32_t	i;

	i = 0;
	if (!split)
		return (0);
	while (*split)
	{
		split++;
		i++;
	}
	return (i);
}

char	*join_splits(char **split, char *join)
{
	size_t	total_length;
	char	*str;
	char	**temp;

	total_length = 0;
	temp = split;
	while (*temp != NULL)
	{
		total_length += ft_strlen(*temp);
		if (*(temp + 1) != NULL)
			total_length += ft_strlen(join);
		temp++;
	}
	str = (char *)malloc(sizeof(char) * (total_length + 1));
	if (str == NULL)
		return (NULL);
	str[0] = '\0';
	while (*split != NULL)
	{
		ft_strcat(str, *split);
		if (*(split + 1) != NULL)
			ft_strcat(str, join);
		split++;
	}
	return (str);
}

t_env_cpy	*init_env(t_process *data)
{
	char		**split_on_equal;
	t_env_cpy	*head;
	t_env_cpy	*current;
	int			i;
	int32_t		count;
	char		*tmp_str;

	if (!data->env)
		return (NULL);
	i = 0;
	split_on_equal = ft_split(data->env[i], '=');
	count = count_splits(split_on_equal);
	if (count > 2)
	{
		tmp_str = join_splits(&split_on_equal[1], "=");
		current = new_env(split_on_equal[0], tmp_str);
		if (tmp_str)
			free(tmp_str);
	}
	else
		current = new_env(split_on_equal[0], split_on_equal[1]);
	if (split_on_equal)
		free_2d_char_array(split_on_equal);
	i++;
	head = current;
	while (data->env[i])
	{
		split_on_equal = ft_split(data->env[i], '=');
		count = count_splits(split_on_equal);
		tmp_str = join_splits(&split_on_equal[1], "=");
		if (count > 2)
			current->next = new_env(split_on_equal[0], tmp_str);
		else
			current->next = new_env(split_on_equal[0], split_on_equal[1]);
		if (tmp_str)
			free(tmp_str);
		if (current->next)
			current->next->prev = current;
		if (split_on_equal)
			free_2d_char_array(split_on_equal);
		current = current->next;
		i++;
	}
	return (head);
}
