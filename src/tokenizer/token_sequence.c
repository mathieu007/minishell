/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/07 07:20:38 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief sequence token are tokens that separate the commands
/// such as &&, ||, <<, |, >>, >, <, ;
/// @param str 
/// @param i 
/// @param type 
/// @param parent 
/// @return 
int32_t	add_token_sequence(char *str, int32_t i, t_token_type type,
	t_token *parent)
{
	add_tk(str, type, i, parent);
	i += get_token_len(&str[i], type, false);
	while (str[i] && str[i] == ' ')
		i++;
	return (i);
}

inline bool	is_token_group(t_token_type type)
{
	return (type == TK_DOUBLEQUOTE || type == TK_SINGLEQUOTE
		|| type == TK_PARENTHESE_OPEN || type == TK_COMMANDSUBSTITUTION_OPEN
		|| type == TK_ENVIRONEMENT_VAR);
}

/// @brief token group are token that may contains other tokens
/// we skip them and will process them on next level.
/// @param str 
/// @param type 
/// @param i 
/// @return 
static int32_t	skip_token_group(char *str, t_token_type type, int32_t i)
{
	if (type == TK_DOUBLEQUOTE)
		i = goto_closing_double_quote(str, i + 1) + 1;
	else if (type == TK_SINGLEQUOTE)
		i = goto_closing_single_quote(str, i + 1) + 1;
	else if (type == TK_COMMANDSUBSTITUTION_OPEN)
		i = goto_closing_parenthese(str, i + 1) + 1;
	else if (type == TK_ENVIRONEMENT_VAR)
		i = goto_closing_environement(str, i + 1) + 1;
	else if (type == TK_PARENTHESE_OPEN)
		i = goto_closing_parenthese(str, i) + 1;
	return (i);
}

/// @brief cmd sequence tokens are tokens that separate commands
/// we add a "false token" at the end and start of the tokenization
/// it is just a way identify and split tokens easily.
/// @param parent 
/// @return 
t_token	*tokenize_cmd_sequence(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*str;
	t_token			*token;

	i = 0;
	str = ft_strtrim(parent->str, " ");
	token = add_tk(ft_strdup(""), TK_START, 0, parent);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (is_token_group(type))
			i = skip_token_group(str, type, i);
		else if (is_end_of_seq(type))
			i = add_token_sequence(ft_substr(&str[i], 0, t_len), i, type, parent);
		else
			i += t_len;
	}
	add_tk(ft_strdup(""), TK_END, i, parent);
	split_token_sequence(parent);
	return (token);
}
