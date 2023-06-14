/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_group.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/14 07:55:24 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief this is the second level of tokenization
/// this level of tokenization will tokenize all
/// the double quotes, single quotes, parentheses, substitution
/// and curlybrace but also spaces.
/// @param high
/// @return 
t_token	*tokenize_group_tokens(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*str;
	t_token			*start_token;

	i = 0;
	str = parent->str;
	start_token = add_tk(ft_strdup(""), TK_START, 0, parent);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (is_token_group(type))
			i = add_token_group(str, i, type, parent);
		else if (type == TK_SPACE)
			i = add_token_space(str, i, parent);
		else
			i += t_len;
	}
	add_tk(ft_strdup(""), TK_END, i, parent);
	split_token_groups(parent);
	return (start_token);
}
