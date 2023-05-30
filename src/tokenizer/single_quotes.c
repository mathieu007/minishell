/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/29 16:50:05 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	get_token_single_quote_len(char *str)
{
	int32_t	len;

	len = 0;
	len++;
	while (str[len] && str[len] != '\'')
		len++;
	return (len);
}

int32_t	add_token_single_quote(int32_t i, t_token_type type, t_token_group *group)
{
	t_token	*token;

	token = add_token(i, type, group);
	token->token_len = 1;
	token->token_str = ft_strdup("'");
	token->end = i + 1;
	return (token->end);
}

int32_t	tokenize_single_quote(char *str, int32_t i, t_token_group *group)
{
	t_token_type	type;
	int32_t			t_len;

	if (str == NULL)
		return (i);
	i = add_token_single_quote(i++, TK_SINGLEQUOTE, group);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, true);
		if (type == TK_SINGLEQUOTE)
			return (add_token_single_quote(i, TK_CLOSINGSINGLEQUOTE, group));
		else
			i += t_len;
	}
	return (i);
}
