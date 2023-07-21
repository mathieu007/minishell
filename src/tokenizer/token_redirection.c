/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/20 13:51:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	add_token_redirection(char *str, int32_t i, t_token_type type,
		t_token *parent)
{
	t_token	*token;
	int32_t	len;

	len = get_token_len(&str[i], type, false);
	token = add_token(i, type, parent);
	token->token_len = len;
	token->token_str = ft_substr(str, i, token->token_len);
	if (!token->token_str)
		free_all_and_exit2(errno, "malloc error");
	token->end = i + token->token_len;
	i += token->token_len;
	while (str[i] && str[i] == ' ')
		i++;
	return (i);
}

void	split_token_redirections(t_token *parent)
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
		token->child = expansion_tokenizer(parent);
		token = token->next;
	}
}
