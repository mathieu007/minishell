/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parenthese.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/02 13:08:36 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	add_token_parenthese(char *str, int32_t i, t_token *parent)
{
	int32_t	i_open;

	i_open = i;
	i = goto_closing_parenthese(str, i + 1);
	if (str[i] != ')')
		missing_closing_parenthese_error();
	add_tk("(", TK_PARENTHESE_OPEN, i_open, parent);
	add_tk(")", TK_PARENTHESE_CLOSE, i, parent);
	i++;
	return (i);
}

void	split_token_parentheses(t_token *parent)
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
		if (is_token_redirection(token->type))
			token->child = expansion_tokenizer(token);
		token = token->next;
	}
}

int32_t	search_parenthese(t_token *parent, int32_t i)
{
	t_token_type	type;
	int32_t			t_len;

	while (parent->str[i])
	{
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type);
		if (type != TK_PARENTHESE_OPEN && is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == TK_PARENTHESE_OPEN)
			return (i);
		else
			i += t_len;
	}
	return (i);
}

void	*check_parentheses(t_token *parent)
{
	if (check_parentheses_syntax_error(parent->str))
		return (NULL);
	if (!has_token("(", parent))
		return (NULL);
	return (parent);
}

t_token	*parentheses_tokenizer(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	i = 0;
	if (!check_parentheses(parent))
		return (NULL);
	i = search_parenthese(parent, i);
	i = add_token_parenthese(parent->str, i, parent);
	while (parent->str[i])
	{
		if (has_error())
			return (parent->child);
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type);
		if (is_token_redirection(type))
			i = add_token_redirection(parent->str, i, type, parent);
		else
			i += t_len;
	}
	while (parent->str[i] == ' ')
		i--;
	add_tk("", TK_END, i, parent);
	return (split_token_parentheses(parent), parent->child);
}
