/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_space_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/18 10:19:43 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	get_token_space_len(char *str)
{
	int32_t	pos;

	pos = 1;
	while (str[pos] && str[pos] == ' ')
		pos++;
	return (pos);
}

int32_t	add_token_space(char *str, int32_t pos, t_token *parent)
{
	t_token	*token;

	token = add_token(pos, TK_SPACE, parent);
	token->token_len = get_token_space_len(&str[pos]);
	token->token_str = ft_strdup(" ");
	token->end = pos + token->token_len;
	return (token->end);
}

/// @brief token group are token that may contains other tokens
/// we skip them and will process them on next level.
/// @param str
/// @param type
/// @param i
/// @return
int32_t	skip_token_quotes(char *str, t_token_type type, int32_t i)
{
	if (type == TK_DOUBLEQUOTE)
		i = goto_closing_double_quote(str, i + 1);
	else if (type == TK_SINGLEQUOTE)
		i = goto_closing_single_quote(str, i + 1);
	return (i);
}

int32_t	add_sgl_quote_token(char *str, int32_t i, t_token *parent)
{
	int32_t	i_open;

	i_open = i;
	i = goto_closing_single_quote(str, i + 1);
	add_tk("'", TK_SINGLEQUOTE, i_open, parent);
	add_tk("'", TK_CLOSINGSINGLEQUOTE, i, parent);
	i++;
	return (i);
}

int32_t	add_dbl_quote_token(char *str, int32_t i, t_token *parent)
{
	int32_t	i_open;

	i_open = i;
	i = goto_closing_double_quote(str, i + 1);
	add_tk("\"", TK_DOUBLEQUOTE, i_open, parent);
	add_tk("\"", TK_CLOSINGDOUBLEQUOTE, i, parent);
	i++;
	return (i);
}

void	split_token_space_quotes(t_token *parent)
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
		token->str = ft_substr(str, start, len);
		token = token->next;
	}
}

t_token	*space_quotes_tokenizer(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*str;

	i = 0;
	str = parent->str;
	add_tk("", TK_START, 0, parent);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (type == TK_SINGLEQUOTE)
			i = add_sgl_quote_token(str, i, parent);
		else if (type == TK_DOUBLEQUOTE)
			i = add_dbl_quote_token(str, i, parent);
		else if (type == TK_SPACE)
			i = add_token_space(str, i, parent);
		else
			i += t_len;
	}
	add_tk("", TK_END, i, parent);
	split_token_space_quotes(parent);
	return (parent->child);
}
