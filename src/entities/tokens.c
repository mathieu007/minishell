/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/11 11:37:01 by mroy             ###   ########.fr       */
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
	new->len = 0;
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
		return (get_first_token());
	return (get_data()->last_token);
}

t_token	*add_token(char *str, int32_t char_pos, t_token_type type, t_token_group *group)
{
	t_token		*last;
	t_token		*new;

	last = get_last_token();
	if (!last->prev)
		new = last;
	else
	{
		new = new_token();
		if (new == NULL)
			return (NULL);
		last->next = new;
		new->prev = last;
	}
	increment_counter(type);
	new->len = get_token_type_len(type);
	new->start = &str[char_pos];
	new->pos = char_pos;
	if (!group->first)
		group->first = new;
	group->last = new;
	get_data()->tokens_count++;
	get_data()->last_token = new;
	return (new);
}
