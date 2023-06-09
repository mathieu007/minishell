/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dash.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/05 14:44:58 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int32_t	add_token_dash(char *str, int32_t pos, t_token *token)
{
	token = add_token(pos, TK_DASH, token);
	token->token_len = get_token_dash_len(&str[pos]);
	token->token_str = ft_substr(str, pos, token->token_len);
	token->end = pos + token->token_len;
	return (token->end);
}

int32_t	get_token_dashdash_len(char *str)
{
	int32_t	i;

	i = 2;
	while (str[i] && ft_isalnum(str[i]) == 1)
		i++;
	return (i);
}

int32_t	get_token_dash_len(char *str)
{
	int32_t	pos;

	pos = 1;
	while (str[pos] && str[pos] != ' ')
		pos++;
	return (pos);
}

int32_t	add_token_dashdash(char *str, int32_t pos, t_token *token)
{
	token = add_token(pos, TK_DASHDASH, token);
	token->token_len = get_token_dashdash_len(&str[pos]);
	token->end = pos + token->token_len;
	token->token_str = ft_substr(str, pos, token->token_len);
	return (token->end);
}
