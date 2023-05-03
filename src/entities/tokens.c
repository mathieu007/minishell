/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 13:49:49 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token_after(t_token *curr)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->value = NULL;
	new->next = NULL;
	if (curr != NULL)
	{
		if (curr->next)
			new->next = curr->next;
		curr->next = new;
		new->prev = curr;		
	}	
	return (new);
}

inline t_token	*get_first_token(void)
{
	static t_token	*token;

	if (token == NULL)
	{
		token = new_token(NULL, NULL);
		get_data()->last_token = token;
		return (token);
	}		
	return (&token[0]);
}
/// @brief TODO
/// @param  
/// @return 
inline t_token	*get_last_token(void)
{
	t_token	*token;
	
	return (&token[0]);
}

static void	*_set_token(t_token *token, char *token_value, int32_t char_pos,
	t_token_type type)
{
	if (token == NULL)
		return (NULL);
	token->pos = char_pos;
	token->index = get_data()->token_count;
	token->len = ft_strlen(token_value);
	token->next = NULL;
	token->type = type;
	token->value = token_value;
	return (token);
}

t_token	*add_token(char *token_value, int32_t char_pos, t_token_type type)
{
	t_token		*last;

	last = get_data()->last_token;
	if (last->prev != NULL)
		last = new_token_after(last);
	increment_counter(type);
	_set_token(last, &token_value[char_pos], char_pos, type);
	get_data()->token_count++;
	get_data()->last_token = last;
	return (last);
}
