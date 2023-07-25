/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 17:31:35 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_token_sequence(t_token *parent)
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
		token->str = ft_strtrimfree(ft_substr(str, start, len), " ");
		token->child = dispatch_tokenizer(token);
		token = token->next;
	}
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

int32_t	add_sequence_token(int32_t i, char *tk_str, t_token_type type,
		t_token *parent)
{
	int32_t	len;

	len = ft_strlen(tk_str);
	add_tk(tk_str, type, i, parent);
	i += len;
	return (i);
}

int32_t	add_semicolon_sequence_token(int32_t i, t_token_type type,
		t_token *parent)
{
	add_tk(";", type, i, parent);
	i++;
	return (i);
}

int32_t	process_semicolon_token(int32_t i, t_token *parent,
		t_token_type tk_type)
{
	t_token_type	type;
	int32_t			t_len;

	type = get_token_type(&parent->str[i]);
	t_len = get_token_len(&parent->str[i], type);
	if (tk_type != type && is_token_delimiter(type))
		i = skip_token_delimiter(type, i, parent);
	else if (type == tk_type)
		i = add_semicolon_sequence_token(i, type, parent);
	else
		i += t_len;
	return (i);
}

t_token	*sequence_semicolon_tokenizer(t_token *parent)
{
	int32_t			i;
	t_token_type	tk_type;

	i = 0;
	i = 0;
	tk_type = get_token_type(";");
	if (!has_token_semicolon_sequence(parent))
		return (NULL);
	add_sequence_token(i, "", TK_START, parent);
	while (parent->str[i])
	{
		if (has_error())
			return (parent->child);
		i = process_semicolon_token(i, parent, tk_type);
		if (i == -1)
			return (parent->child);
	}
	add_tk("", TK_END, i, parent);
	split_token_sequence(parent);
	return (parent->child);
}


// t_token	*sequence_semicolon_tokenizer(t_token *parent)
// {
// 	int32_t			i;
// 	t_token_type	type;
// 	int32_t			t_len;
// 	t_token_type	tk_type;

// 	i = 0;
// 	tk_type = get_token_type(";");
// 	if (!has_token_semicolon_sequence(parent))
// 		return (NULL);
// 	add_sequence_token(i, "", TK_START, parent);
// 	while (parent->str[i])
// 	{
// 		if (has_error())
// 			return (parent->child);
// 		type = get_token_type(&parent->str[i]);
// 		t_len = get_token_len(&parent->str[i], type);
// 		if (tk_type != type && is_token_delimiter(type))
// 			i = skip_token_delimiter(type, i, parent);
// 		else if (type == tk_type)
// 			i = add_semicolon_sequence_token(i, type, parent);
// 		else
// 			i += t_len;
// 	}
// 	add_tk("", TK_END, i, parent);
// 	split_token_sequence(parent);
// 	return (parent->child);
// }
