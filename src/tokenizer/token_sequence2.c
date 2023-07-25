/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 17:33:09 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	process_sequence(int32_t i, t_token *parent, t_token_type tk_type,
		char *tk)
{
	t_token_type	type;
	int32_t			t_len;

	type = get_token_type(&parent->str[i]);
	t_len = get_token_len(&parent->str[i], type);
	if (tk_type != type && is_token_delimiter(type))
		i = skip_token_delimiter(type, i, parent);
	else if (type == tk_type)
		i = add_sequence_token(i, tk, type, parent);
	else
		i += t_len;
	return (i);
}

t_token	*sequence_tokenizer(char *tk, t_token *parent)
{
	int32_t			i;
	t_token_type	tk_type;

	i = 0;
	i = 0;
	tk_type = get_token_type(tk);
	if (!has_token_sequence(parent))
		return (NULL);
	add_sequence_token(i, "", tk_type, parent);
	while (parent->str[i])
	{
		if (has_error())
			return (parent->child);
		i = process_sequence(i, parent, tk_type, tk);
		if (i == -1)
			return (parent->child);
	}
	add_tk("", TK_END, i, parent);
	split_token_sequence(parent);
	return (parent->child);
}

bool	is_any_of(char c, char *values)
{
	while (*values)
	{
		if (*values == c)
			return (true);
		values++;
	}
	return (false);
}

// t_token	*sequence_tokenizer(char *tk, t_token *parent)
// {
// 	int32_t			i;
// 	t_token_type	type;
// 	int32_t			t_len;
// 	t_token_type	tk_type;

// 	i = 0;
// 	tk_type = get_token_type(tk);
// 	if (!has_token_sequence(parent))
// 		return (NULL);
// 	add_sequence_token(i, "", tk_type, parent);
// 	while (parent->str[i])
// 	{
// 		if (has_error())
// 			return (parent->child);
// 		type = get_token_type(&parent->str[i]);
// 		t_len = get_token_len(&parent->str[i], type);
// 		if (tk_type != type && is_token_delimiter(type))
// 			i = skip_token_delimiter(type, i, parent);
// 		else if (type == tk_type)
// 			i = add_sequence_token(i, tk, type, parent);
// 		else
// 			i += t_len;
// 	}
// 	add_tk("", TK_END, i, parent);
// 	split_token_sequence(parent);
// 	return (parent->child);
// }

t_token	*sequences_tokenizer(t_token *parent)
{
	t_token	*child;

	child = NULL;
	if (has_error())
		return (parent->child);
	if (has_token(";", parent))
		child = sequence_semicolon_tokenizer(parent);
	else if (has_token("||", parent))
		child = sequence_tokenizer("||", parent);
	else if (has_token("&&", parent))
		child = sequence_tokenizer("&&", parent);
	else if (has_token("|", parent))
		child = sequence_tokenizer("|", parent);
	return (child);
}
