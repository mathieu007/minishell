/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/19 10:31:53 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_group	*new_token_group()
{
	t_token_group	*new;

	new = ft_calloc(1, sizeof(t_token_group));
	if (!new)
		return (NULL);	
	return (new);
}

t_token_group	*add_token_group(char *start, t_token_type type, int32_t len)
{
	t_token_group	*last;
	t_token_group	*new;
	t_data			*data;

	data = get_data();
	last = data->last_token_group;
	new = new_token_group();
	if (!new)
		return (NULL);
	if (last)
	{
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
