/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/07/17 18:09:38 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline t_token_type	get_token_type(char *str)
{
	t_token_type	*lookup_table;
	t_token_type	tk_type;

	if (!str)
		return (TK_UNKNOWN);
	lookup_table = get_tokens_lookup_table();
	tk_type = lookup_table[(int32_t)(str[0] * 255 + str[1])];
	if (tk_type != TK_UNKNOWN)
		return (tk_type);
	tk_type = lookup_table[(int32_t)str[0]];
	return (tk_type);
}

static void	init_token_types_1(t_token_type *tk_type)
{
	tk_type[TK_CMD] = TK_CMD;
	tk_type[TK_START] = TK_START;
	tk_type[TK_END] = TK_END;
	tk_type[TK_SPACE] = TK_SPACE;
	tk_type[TK_AMPERSAND] = TK_AMPERSAND;
	tk_type[TK_AND] = TK_AND;
	tk_type[TK_BACKSLASH] = TK_BACKSLASH;
	tk_type[TK_BACKSLASHBACKSLASH] = TK_BACKSLASHBACKSLASH;
	tk_type[TK_OR] = TK_OR;
	tk_type[TK_DASH] = TK_DASH;
	tk_type[TK_DASHDASH] = TK_DASHDASH;
	tk_type[TK_DOLLAR_SIGN] = TK_ENVIRONEMENT_VAR;
	tk_type[TK_DOLLAR_SIGN_CURLYBRACE] = TK_ENVIRONEMENT_VAR;
	tk_type[TK_DOUBLEQUOTE] = TK_DOUBLEQUOTE;
	tk_type[TK_GREAT] = TK_GREAT;
}

static void	init_token_types_2(t_token_type *tk_type)
{
	tk_type[TK_GREATGREAT] = TK_GREATGREAT;
	tk_type[TK_LESS] = TK_LESS;
	tk_type[TK_LESSLESS] = TK_LESSLESS;
	tk_type[TK_LAST_CMD_EXIT] = TK_LAST_CMD_EXIT;
	tk_type[TK_PIPE] = TK_PIPE;
	tk_type[TK_SINGLEQUOTE] = TK_SINGLEQUOTE;
	tk_type[TK_WILDCARD] = TK_WILDCARD;
	tk_type[TK_SEMICOLON] = TK_SEMICOLON;
	tk_type[TK_BACKSLASHDOUBLEQUOTE] = TK_BACKSLASHDOUBLEQUOTE;
	tk_type[TK_BACKSLASHDOLLARSIGN] = TK_BACKSLASHDOLLARSIGN;
	tk_type[TK_BACKSLASHLINEFEED] = TK_BACKSLASHLINEFEED;
	tk_type[TK_BACKSLASHTAB] = TK_BACKSLASHTAB;
	tk_type[TK_COMMANDSUBSTITUTION_OPEN] = TK_COMMANDSUBSTITUTION_OPEN;
	tk_type[TK_COMMANDSUBSTITUTION_CLOSE] = TK_COMMANDSUBSTITUTION_CLOSE;
}

static void	init_token_types_3(t_token_type *tk_type)
{
	tk_type[TK_PARENTHESE_OPEN] = TK_PARENTHESE_OPEN;
	tk_type[TK_PARENTHESE_CLOSE] = TK_PARENTHESE_CLOSE;
	tk_type[TK_GROUP_REDIRECTIONS] = TK_GROUP_REDIRECTIONS;
	tk_type[TK_CLOSING_CURLYBRACE] = TK_CLOSING_CURLYBRACE;
	tk_type[TK_DOLLAR_SIGN_CURLYBRACE] = TK_DOLLAR_SIGN_CURLYBRACE;
}

inline t_token_type	*get_tokens_lookup_table(void)
{
	static t_token_type	token_types[(255 * 255) + (255 * 2)];
	static int32_t		i;
	t_token_type		*tk_type;

	tk_type = &token_types[10];
	if (i == 0)
	{
		while (i < (255 * 255) + (255 * 2))
		{
			token_types[i] = TK_UNKNOWN;
			i++;
		}
		init_token_types_1(tk_type);
		init_token_types_2(tk_type);
		init_token_types_3(tk_type);
	}
	return (tk_type);
}

// static inline t_token_type	*get_tokens_lookup_table(void)
// {
// 	static t_token_type	token_types[(255 * 255) + (255 * 2)];
// 	static int32_t		i;
// 	t_token_type		*tk_type;

// 	tk_type = &token_types[10];
// 	if (i == 0)
// 	{
// 		while (i < (255 * 255) + (255 * 2))
// 		{
// 			token_types[i] = TK_UNKNOWN;
// 			i++;
// 		}
// 		tk_type[TK_CMD] = TK_CMD;
// 		tk_type[TK_START] = TK_START;
// 		tk_type[TK_END] = TK_END;
// 		tk_type[TK_SPACE] = TK_SPACE;
// 		tk_type[TK_AMPERSAND] = TK_AMPERSAND;
// 		tk_type[TK_AND] = TK_AND;
// 		tk_type[TK_BACKSLASH] = TK_BACKSLASH;
// 		tk_type[TK_BACKSLASHBACKSLASH] = TK_BACKSLASHBACKSLASH;
// 		tk_type[TK_OR] = TK_OR;
// 		tk_type[TK_DASH] = TK_DASH;
// 		tk_type[TK_DASHDASH] = TK_DASHDASH;
// 		tk_type[TK_DOLLAR_SIGN] = TK_ENVIRONEMENT_VAR;
// 		tk_type[TK_DOLLAR_SIGN_CURLYBRACE] = TK_ENVIRONEMENT_VAR;
// 		tk_type[TK_DOUBLEQUOTE] = TK_DOUBLEQUOTE;
// 		tk_type[TK_GREAT] = TK_GREAT;
// 		tk_type[TK_GREATGREAT] = TK_GREATGREAT;
// 		tk_type[TK_LESS] = TK_LESS;
// 		tk_type[TK_LESSLESS] = TK_LESSLESS;
// 		tk_type[TK_LAST_CMD_EXIT] = TK_LAST_CMD_EXIT;
// 		tk_type[TK_PIPE] = TK_PIPE;
// 		tk_type[TK_SINGLEQUOTE] = TK_SINGLEQUOTE;
// 		tk_type[TK_WILDCARD] = TK_WILDCARD;
// 		tk_type[TK_SEMICOLON] = TK_SEMICOLON;
// 		tk_type[TK_BACKSLASHDOUBLEQUOTE] = TK_BACKSLASHDOUBLEQUOTE;
// 		tk_type[TK_BACKSLASHDOLLARSIGN] = TK_BACKSLASHDOLLARSIGN;
// 		tk_type[TK_BACKSLASHLINEFEED] = TK_BACKSLASHLINEFEED;
// 		tk_type[TK_BACKSLASHTAB] = TK_BACKSLASHTAB;
// 		tk_type[TK_COMMANDSUBSTITUTION_OPEN] = TK_COMMANDSUBSTITUTION_OPEN;
// 		tk_type[TK_COMMANDSUBSTITUTION_CLOSE] = TK_COMMANDSUBSTITUTION_CLOSE;
// 		tk_type[TK_PARENTHESE_OPEN] = TK_PARENTHESE_OPEN;
// 		tk_type[TK_PARENTHESE_CLOSE] = TK_PARENTHESE_CLOSE;
// 		tk_type[TK_GROUP_REDIRECTIONS] = TK_GROUP_REDIRECTIONS;
// 		tk_type[TK_CLOSING_CURLYBRACE] = TK_CLOSING_CURLYBRACE;
// 		tk_type[TK_DOLLAR_SIGN_CURLYBRACE] = TK_DOLLAR_SIGN_CURLYBRACE;
// 	}
// 	return (tk_type);
// }