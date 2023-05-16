/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 08:36:41 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_group	*new_token_group()
{
	t_token_group	*new;

	new = malloc(sizeof(t_token_group));
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
	static t_token_group	*group;

	if (group == NULL)
	{
		group = new_token_group();
		get_data()->last_token_group = group;
		return (group);
	}		
	return (&group[0]);
}

inline t_token_group	*get_last_token_group(void)
{
	if (!get_data()->token_groups)
		return (get_first_token_group());	
	return (get_data()->token_groups);
}

inline t_token_group	*get_token_group_at(int32_t index)
{
	t_token_group	*token;
	int32_t			i;

	token = get_first_token_group();
	i = 0;
	while (token && i < index)
	{
		token = token->next;
		i++;
	}
	return (token);
}

t_token_group	*add_token_group(char *start, int32_t len)
{
	t_token_group	*last;
	t_token_group	*new;

	last = get_data()->last_token_group;
	if (!last)
		new = get_first_token_group();
	else
	{
		new = new_token_group();
		if (new == NULL)
			return (NULL);
		last->next = new;
		new->prev = last;
	}
	new->start = ft_strncpy(start, len);
	new->len = len;
	get_data()->token_groups_count++;
	get_data()->last_token_group = new;
	return (new);
}
