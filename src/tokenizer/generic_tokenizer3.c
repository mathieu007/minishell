/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_tokenizer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_token_semicolon_sequence(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	if (get_process()->syntax_error)
		return (false);
	i = 0;
	while (parent->str[i])
	{
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == TK_SEMICOLON)
			return (true);
		else
			i += t_len;
	}
	return (false);
}

bool	has_token_sequence(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	if (get_process()->syntax_error)
		return (false);
	i = 0;
	while (parent->str[i])
	{
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == TK_OR || type == TK_AND || type == TK_PIPE)
			return (true);
		else
			i += t_len;
	}
	return (false);
}

bool	has_token(char *tk, t_token *parent)
{
	int32_t			i;
	t_token_type	cur_tk;
	int32_t			t_len;
	t_token_type	type;

	if (get_process()->syntax_error)
		return (false);
	i = 0;
	type = get_token_type(tk);
	while (parent->str[i])
	{
		cur_tk = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], cur_tk, false);
		if (type != cur_tk && is_token_delimiter(cur_tk))
			i = skip_token_delimiter(cur_tk, i, parent);
		else if (cur_tk == type)
			return (true);
		else
			i += t_len;
	}
	return (false);
}

t_token	*dispatch_tokenizer(t_token *parent)
{
	t_token	*child;

	if (has_error())
		return (NULL);
	child = sequences_tokenizer(parent);
	if (!child)
		child = parentheses_tokenizer(parent);
	if (!child)
		child = cmd_tokenizer(parent);
	return (child);
}
