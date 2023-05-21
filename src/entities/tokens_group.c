/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/21 08:33:26 by math             ###   ########.fr       */
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

void	reset_token_group(t_token_group *group)
{
	if (!group)
		return ;
	free_t_tokens(group->first_token);
	group->first_token = NULL;
	group->last_token = NULL;
	free(group->str);
}
