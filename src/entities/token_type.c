/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/05/11 11:17:18 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	*get_tokens_lookup_table(void)
{
	static t_token_type	token_types[255 * 255];
	static int32_t		i;

	if (i == 0)
	{
		while (i < 255 * 255)
		{
			token_types[i] = TK_UNKNOWN;
			i++;
		}
		token_types[TK_AMPERSAND] = TK_AMPERSAND;
		token_types[TK_AND] = TK_AND;
		token_types[TK_BACKSLASH] = TK_BACKSLASH;
		token_types[TK_OR] = TK_OR;
		token_types[TK_DASH] = TK_DASH;
		token_types[TK_DASHDASH] = TK_DASHDASH;
		token_types[TK_DOLLAR_SIGN] = TK_DOLLAR_SIGN;
		token_types[TK_DOUBLEQUOTE] = TK_DOUBLEQUOTE;
		token_types[TK_END] = TK_END;
		token_types[TK_GREAT] = TK_GREAT;
		token_types[TK_GREATGREAT] = TK_GREATGREAT;
		token_types[TK_LESS] = TK_LESS;
		token_types[TK_LESSLESS] = TK_LESSLESS;
		token_types[TK_LAST_PIPE_EXIT] = TK_LAST_PIPE_EXIT;
		token_types[TK_PIPE] = TK_PIPE;
		token_types[TK_SINGLEQUOTE] = TK_SINGLEQUOTE;
		token_types[TK_VAR_ASSIGN] = TK_VAR_ASSIGN;
		token_types[TK_WILDCARD] = TK_WILDCARD;
		token_types[TK_SEMICOLON] = TK_SEMICOLON;
	}
	return (&token_types[0]);
}

t_token_type	get_token_type(char *str)
{
	t_token_type	*lookup_table;
	t_token_type	tk_type;

	if (!str || *str == '\0')
		return (TK_UNKNOWN);
	if (*str == '\0')
		return (TK_END);
	lookup_table = get_tokens_lookup_table();
	tk_type = lookup_table[(int32_t)(str[0] * str[1] + str[1])];
	if (str[1] != '\0' && tk_type != TK_UNKNOWN)
		return (tk_type);
	tk_type = lookup_table[(int32_t)str[0]];
	return (tk_type);
}
