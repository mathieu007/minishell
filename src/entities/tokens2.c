/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/06/26 15:41:18 by mroy             ###   ########.fr       */
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
		last = get_last(parent->child);
	else
		last = get_last(get_process()->tokens);
	new = new_token();
	new->parent = parent;
	if (!new)
		return (NULL);
	if (last)
	{
		last->next = new;
		new->prev = last;
	}
	new->start = pos;
	new->type = type;
	new->is_redirection = false;
	if (!parent && !get_process()->tokens)
		get_process()->tokens = new;
	else if (parent && !parent->child)
		parent->child = new;
	if (parent)
		parent->last = new;
	return (new);
}
