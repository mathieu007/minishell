/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/01 15:26:46 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token_after(t_token *curr)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->value == NULL;
	token->next = NULL;
	if (token == NULL)
		return (NULL);
	if (curr != NULL)
		curr->next = token;
		token->prev = curr;
	return (token);
}

inline t_token	*get_tokens(void)
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

static void	*_set_token(t_token *token, char *token_value, int32_t char_pos,
	t_token_type type)
{
	if (token == NULL)
		return (NULL);
	token->start_pos = char_pos;
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
