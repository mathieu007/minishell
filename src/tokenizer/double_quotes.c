/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/31 10:04:30 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief this function just validate that the token is a valid toke inside 
/// double quotes.
/// @param type 
/// @return 
static bool	is_valid_in_dbl_quote(t_token_type type)
{
	return (type == TK_BACKSLASHDOUBLEQUOTE
		|| type == TK_BACKSLASHLINEFEED
		|| type == TK_BACKSLASHBACKSLASH
		|| type == TK_BACKSLASHTAB);
}

static int32_t	add_other_token(char *token_str, t_token_type type,
	int32_t i, t_token_sequence *group)
{
	t_token			*token;

	token = add_token(i, type, group);
	token->token_len = ft_strlen(token_str);
	token->token_str = token_str;
	token->end = i + token->token_len;
	return (token->end);
}


static int32_t	add_token_double_quote(int32_t i, t_token_type type,
	t_token_sequence *group)
{
	t_token			*token;

	token = add_token(i, type, group);
	token->token_len = 1;
	token->token_str = ft_strdup("\"");
	token->end = i + 1;
	return (token->end);
}

int32_t	tokenize_double_quote(char *str, int32_t i, t_token_sequence *group)
{
	t_token_type	type;
	int32_t			t_len;	

	i = add_token_double_quote(i, TK_DOUBLEQUOTE, group);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, true);
		if (type == TK_DOUBLEQUOTE)
			return (add_token_double_quote(i, TK_CLOSINGDOUBLEQUOTE, group));
		else if (str_is_env_variable(&str[i]))
			i = add_token_env(str, i, group, true);
		else if (is_valid_in_dbl_quote(type))
			i = add_other_token(ft_substr(&str[i], 0, t_len), type, i, group);
		else
			i += t_len;
	}
	return (i);
}
