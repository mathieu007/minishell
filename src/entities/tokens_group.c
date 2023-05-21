/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/17 16:23:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_group	*new_token_group()
{
	t_token_group	*new;

	new = ft_calloc(1 ,sizeof(t_token_group));
	if (new == NULL)
		return (NULL);
	new->token_count = 0;
	new->first_token = NULL;
	new->next = NULL;
	new->prev = NULL;
	
	return (new);
}

inline t_token_group	*get_first_token_group(void)
{
	t_token_group	*group;
	t_data			*data;

	data = get_data();
	group = data->token_groups;
	if (group == NULL)
	{
		group = new_token_group();
		data->token_groups = group;
		data->last_token_group = group;
		return (group);
	}		
	return (group);
}

t_token_group	*add_token_group(char *start, t_token_type type, int32_t len)
{
	t_token_group	*last;
	t_token_group	*new;
	t_data			*data;

	data = get_data();
	last = data->last_token_group;
	new = new_token_group();
	if (last)
	{
		new = new_token_group();
		if (new == NULL)
			return (NULL);
		last->next = new;
		new->prev = last;		
	}
	else
		data->token_groups = new;
	new->str = ft_strncpy(start, len);
	new->len = len;
	new->cmd_seq_type = get_sequence_type(type);
	data->token_groups_count++;
	data->last_token_group = new;
	return (new);
}
