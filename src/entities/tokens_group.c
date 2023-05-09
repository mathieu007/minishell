/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 06:46:06 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_group	*new_token_group(char *str)
{
	t_token_group	*new;

	new = malloc(sizeof(t_token_group));
	if (new == NULL)
		return (NULL);
	new->start = str;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

inline t_token_group	*get_first_token_group(void)
{
	static t_token_group	*token;

	if (token == NULL)
	{
		token = new_token_group(NULL);
		return (token);
	}		
	return (&token[0]);
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

static void	*_set_token(t_token *token, char *token_value, int32_t char_pos,
	t_token_type type)
{
	if (token == NULL)
		return (NULL);
	token->pos = char_pos;
	if (((int32_t)type) < 255)
		token->len = 1;
	else
		token->len = 2;
	token->next = NULL;
	token->type = type;
	token->value = token_value;
	return (token);
}

t_token_group	*add_token_group(char *str, int32_t char_pos,
	t_token_type type)
{
	t_token_group		*last;

	last = get_data()->last_token_group;
	if (last->prev != NULL)
		last = new_token_group(last, str);
	if (is_end_cmd_tk(last) && last->prev)
		last->start = &token_value[char_pos];
	increment_counter(type);
	_set_token(last, &token_value[char_pos], char_pos, type);
	get_data()->token_count++;
	get_data()->last_token = last;
	return (last);
}
