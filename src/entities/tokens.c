/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/17 21:15:44 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(void)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->token_len = 0;
	return (new);
}

t_token	*add_token(int32_t pos, t_token_type type, t_token_group *group)
{
	t_token	*last;
	t_token	*new;

	last = group->last_token;
	new = new_token();
	if (last)
	{		
		if (!new)
			return (NULL);
		last->next = new;
		last->tolal_len = pos - last->pos;
		new->prev = last;
	}
	increment_counter(type);
	new->pos = pos;
	new->type = type;
	new->token_len = get_token_type_len(type);
	new->tolal_len = 1;
	if (!group->first_token)
		group->first_token = new;
	group->last_token = new;
	get_data()->tokens_count++;
	return (new);
}
