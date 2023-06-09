/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_space.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/06 13:42:35 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	get_token_space_len(char *str)
{
	int32_t	pos;

	pos = 1;
	while (str[pos] && str[pos] == ' ')
		pos++;
	return (pos);
}

int32_t	add_token_space(char *str, int32_t pos, t_token *parent)
{
	t_token			*token;

	token = add_token(pos, TK_SPACE, parent);
	token->token_len = get_token_space_len(&str[pos]);
	token->token_str = ft_strdup(" ");
	token->end = pos + token->token_len;
	return (token->end);
}

/// @brief separators are spaces, this function does not execute inside
/// token string.
/// @param parent 
/// @return 
t_token	*tokenize_space_tokens(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*str;

	i = 0;
	str = ft_strtrim(parent->str, " ");
	add_tk(ft_strdup(""), TK_START, 0, parent);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (type == TK_SPACE)
			i = add_token_space(str, i, parent);
		else
			i += t_len;
	}
	add_tk(ft_strdup(""), TK_END, i, parent);
	split_tokens(parent);
	return (parent->child_tokens);
}