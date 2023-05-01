/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/04/29 17:31:21 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	return (token);
}

inline t_token	*get_tokens(void)
{
	static t_token	*token;

	if (token == NULL)
	{
		token = new_token();
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
	t_token		*token;

	token = get_tokens();
	if (token == NULL)
		return (NULL);
	if (token->value == NULL)
	{
		_set_token(token, token_value, char_pos, type);
		token->prev = NULL;
		get_data()->token_count++;
	}
	while (token->next != NULL)
		token = token->next;
	token->next = new_token();
	if (token->next == NULL)
		return (NULL);
	_set_token(token->next, token_value, char_pos, type);
	token->prev = token->next;
	get_data()->token_count++;
	return (token->next);
}

t_token	*remove_token(t_token *tokens)
{
	
}
