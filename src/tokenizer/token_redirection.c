/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/14 11:08:01 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	add_token_redir(char *str, int32_t i, t_token_type type,
	t_token *parent)
{
	t_token			*token;

	token = add_token(i, type, parent);
	token->token_len = get_token_len(&str[i], type, false);
	token->token_str = ft_substr(&str[i], 0, token->token_len);
	if (!token->token_str)
		free_all_and_exit2(errno, "malloc error");
	token->end = i + token->token_len;
	i += token->token_len;
	while (str[i] && str[i] == ' ')
		i++;
	return (i);
}

/// @brief this is the second level of tokenization
/// this level of tokenization will tokenize all
/// the double quotes, single quotes, parentheses, substitution
/// and curlybrace but also spaces.
/// @param high
/// @return 
t_token	*tokenize_redirection(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*str;

	i = 0;
	str = parent->str;
	add_tk(ft_strdup(""), parent->type, i, parent);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (is_token_redir(type))
			i = add_token_redir(str, i, type, parent);
		else
			i += t_len;
	}
	add_tk(ft_strdup(""), TK_END, i, parent);
	split_token_redir(parent);
	return (parent->child_tokens);
}
