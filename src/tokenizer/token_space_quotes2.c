/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_space_quotes2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/01 07:37:51 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_token_space_quotes(t_token *parent)
{
	char	*str;
	int32_t	start;
	int32_t	len;
	t_token	*token;

	token = parent->child;
	str = parent->str;
	while (token && token->next)
	{
		len = token->next->start - token->end;
		start = token->start + token->token_len;
		token->str = free_ptr(token->str);
		token->str = ft_substr(str, start, len);
		token = token->next;
	}
}

t_token	*space_quotes_tokenizer(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*str;

	i = 0;
	str = parent->str;
	if (!str)
		return (NULL);
	add_tk("", TK_START, 0, parent);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type);
		if (type == TK_SINGLEQUOTE)
			i = add_sgl_quote_token(str, i, parent);
		else if (type == TK_DOUBLEQUOTE)
			i = add_dbl_quote_token(str, i, parent);
		else if (type == TK_SPACE)
			i = add_token_space(str, i, parent);
		else
			i += t_len;
	}
	add_tk("", TK_END, i, parent);
	split_token_space_quotes(parent);
	return (parent->child);
}
