/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/05/24 13:36:29 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	*get_tokens_lookup_table(void)
{
	static t_token_type	token_types[(255 * 255) + (255 * 2)];
	static int32_t		i;
	t_token_type 		*tk_type = &token_types[8];

	if (i == 0)
	{
		while (i < (255 * 255) + (255 * 2))
		{
			token_types[i] = TK_UNKNOWN;
			i++;
		}
		tk_type[TK_CMD_SEQ_START] = TK_CMD_SEQ_START;
		tk_type[TK_CMD_SEQ_END] = TK_CMD_SEQ_END;
		tk_type[TK_SPACE] = TK_SPACE;
		tk_type[TK_AMPERSAND] = TK_AMPERSAND;
		tk_type[TK_AND] = TK_AND;
		tk_type[TK_BACKSLASH] = TK_BACKSLASH;
		tk_type[TK_OR] = TK_OR;
		tk_type[TK_DASH] = TK_DASH;
		tk_type[TK_DASHDASH] = TK_DASHDASH;
		tk_type[TK_DOLLAR_SIGN] = TK_DOLLAR_SIGN;
		tk_type[TK_DOUBLEQUOTE] = TK_DOUBLEQUOTE;		
		tk_type[TK_GREAT] = TK_GREAT;
		tk_type[TK_GREATGREAT] = TK_GREATGREAT;
		tk_type[TK_LESS] = TK_LESS;
		tk_type[TK_LESSLESS] = TK_LESSLESS;
		tk_type[TK_LAST_PIPE_EXIT] = TK_LAST_PIPE_EXIT;
		tk_type[TK_PIPE] = TK_PIPE;
		tk_type[TK_SINGLEQUOTE] = TK_SINGLEQUOTE;
		tk_type[TK_WILDCARD] = TK_WILDCARD;
		tk_type[TK_SEMICOLON] = TK_SEMICOLON;
		tk_type[TK_BACKSLASHDOUBLEQUOTE] = TK_BACKSLASHDOUBLEQUOTE;
		tk_type[TK_BACKSLASHSINGLEQUOTE] = TK_BACKSLASHSINGLEQUOTE;		
	}
	return (tk_type);
}

t_token_type	get_token_type(char *str)
{
	t_token_type	*lookup_table;
	t_token_type	tk_type;

	if (!str)
		return (TK_UNKNOWN);
	lookup_table = get_tokens_lookup_table();
	tk_type = lookup_table[(int32_t)(str[0] * str[1] + str[1]) + 1];
	if (tk_type != TK_UNKNOWN)
		return (tk_type);
	tk_type = lookup_table[(int32_t)str[0]];
	return (tk_type);
}
