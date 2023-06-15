/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/14 16:21:41 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	add_token_redir(char *str, int32_t i, t_token_type type,
	t_token *parent)
{
	t_token			*token;

	token = add_token(i, type, parent);
	token->is_redirection = true;
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
	t_token			*start_token;

	i = 0;
	str = ft_strjoin(parent->token_str, parent->str);
	parent->is_redirection = false;
	start_token = add_tk(ft_strdup(parent->token_str), parent->type, i, parent);
	start_token->is_redirection = true;
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
	parent->child_tokens = start_token;
	split_token_redir(parent);
	return (parent->child_tokens);
}
