/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_tokenizer2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 17:33:24 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	check_sgl_quotes_continuation(int32_t i, t_token *parent)
{
	i = goto_closing_single_quote(parent->str, i + 1);
	if (parent->str[i] != '\'')
	{
		exec_delimiter_continuation("\'", parent);
		return (goto_closing_single_quote(parent->str, i));
	}
	return (i);
}

int32_t	check_environement_continuation(int32_t i, t_token *parent)
{
	i = goto_closing_environement(parent->str, i + 2);
	if (parent->str[i] != '}')
	{
		exec_delimiter_continuation("}", parent);
		return (goto_closing_environement(parent->str, i));
	}
	return (i);
}

int32_t	check_parenthese_continuation(int32_t i, t_token *parent)
{
	i = goto_closing_parenthese(parent->str, i + 1);
	if (parent->str[i] != ')')
	{
		exec_delimiter_continuation(")", parent);
		return (goto_closing_parenthese(parent->str, i));
	}
	return (i);
}

int32_t	skip_token_delimiter(t_token_type type, int32_t i, t_token *parent)
{
	if (type == TK_DOUBLEQUOTE)
		i = goto_closing_double_quote(parent->str, i + 1);
	else if (type == TK_SINGLEQUOTE)
		i = goto_closing_single_quote(parent->str, i + 1);
	else if (type == TK_COMMANDSUBSTITUTION_OPEN)
		i = goto_closing_parenthese(parent->str, i + 1);
	else if (type == TK_PARENTHESE_OPEN)
		i = goto_closing_parenthese(parent->str, i + 1);
	else if (type == TK_DOLLAR_SIGN_CURLYBRACE)
		i = goto_closing_environement(parent->str, i + 1);
	return (i + 1);
}

bool	has_token_redirection(t_token *parent)
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
		t_len = get_token_len(&parent->str[i], type);
		if (is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == TK_GREAT || type == TK_GREATGREAT)
			return (true);
		else if (type == TK_LESS || type == TK_LESSLESS)
			return (true);
		else
			i += t_len;
	}
	return (false);
}
