/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/28 13:37:58 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(void)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (new == NULL)
		return (NULL);
	return (new);
}

t_token	*add_token(int32_t pos, t_token_type type, t_token_group *group)
{
	t_token	*last;
	t_token	*new;

	last = group->last_token;
	new = new_token();
	if (!new)
		return (NULL);
	if (last)
	{		
		last->next = new;
		// last->tolal_len = pos + new->token_len - last->start;
		new->prev = last;
	}
	increment_counter(type);
	new->start = pos;
	new->type = type;
	new->str_len = 1;
	if (!group->first_token)
		group->first_token = new;
	group->last_token = new;
	get_process()->tokens_count++;
	return (new);
}
