/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 07:10:32 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token()
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->start = NULL;
	new->next = NULL;
	new->prev = NULL;
	new->token_len = 0;
	new->start = NULL;
	return (new);
}

inline t_token	*get_first_token(void)
{
	static t_token	*token;

	if (token == NULL)
	{
		token = new_token();
		if (token == NULL)
			return (NULL);
		get_data()->last_token = token;
		return (token);
	}		
	return (&token[0]);
}

inline t_token	*get_token_at(int32_t index)
{
	t_token	*token;
	int32_t	i;

	token = get_first_token();
	if (token == NULL)
		return (NULL);
	i = 0;
	while (token && i < index)
	{
		token = token->next;
		i++;
	}
	return (token);
}

/// @brief TODO
/// @param  
/// @return 
inline t_token	*get_last_token(void)
{
	if (!get_data()->last_token)
		return (NULL);
	return (get_data()->last_token);
}

t_token	*add_token(int32_t pos, t_token_type type, t_token_group *group)
{
	t_token		*last;
	t_token		*new;

	last = get_data()->last_token;
	if (!last)
		new = get_first_token();
	else
	{
		new = new_token();
		if (!new)
			return (NULL);
		last->next = new;
		new->prev = last;
	}
	increment_counter(type);
	new->pos = pos;
	new->type = type;
	if (!group->first)
		group->first = new;
	group->last = new;
	get_data()->tokens_count++;
	get_data()->last_token = new;
	return (new);
}
