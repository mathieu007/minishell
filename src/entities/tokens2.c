/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/14 09:05:06 by mroy             ###   ########.fr       */
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

t_token	*get_last(t_token *token)
{
	while (token && token->next)
		token = token->next;
	return (token);
}

t_token	*add_token(int32_t pos, t_token_type type, t_token *parent)
{
	t_token	*last;
	t_token	*new;

	last = NULL;
	if (parent)
		last = get_last(parent->child_tokens);
	else
		last = get_last(get_process()->tokens);
	new = new_token();
	new->parent_token = parent;
	if (!new)
		return (NULL);
	if (last)
	{		
		last->next = new;
		new->prev = last;
	}
	increment_counter(type);
	new->start = pos;
	new->type = type;
	new->contains_redir = false;
	if (!parent && !get_process()->tokens)
		get_process()->tokens = new;
	else if (parent && !parent->child_tokens)
		parent->child_tokens = new;
	if (parent)	
		parent->last = new;
	return (new);
}
