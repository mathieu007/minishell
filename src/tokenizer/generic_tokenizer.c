/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_tokenizer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/01 07:50:44 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	is_token_delimiter(t_token_type type)
{
	return (type == TK_DOUBLEQUOTE || type == TK_SINGLEQUOTE
		|| type == TK_PARENTHESE_OPEN || type == TK_COMMANDSUBSTITUTION_OPEN);
}

inline int32_t	goto_token(char *str, char *tk)
{
	int32_t	i;
	int32_t	i2;
	int32_t	tk_len;

	i = 0;
	tk_len = ft_strlen(tk);
	while (str[i])
	{
		i2 = 0;
		while (i2 < tk_len && tk[i2] == str[i + i2])
		{
			i2++;
			if (i2 == tk_len)
				return (i);
		}
		i++;
	}
	return (-1);
}

int32_t	skip_token_single_quote(char *str, t_token_type type, int32_t i)
{
	if (type == TK_SINGLEQUOTE)
		i = goto_closing_single_quote(str, i + 1);
	return (i + 1);
}

int32_t	check_dbl_quotes_continuation(int32_t i, t_token *parent)
{
	char	*str;

	str = &parent->str[i];
	if (!str)
		return (i + 1);
	i = goto_closing_double_quote(parent->str, i + 1);
	if (parent->str[i] != '"')
	{
		exec_delimiter_continuation("\"", parent);
		return (goto_closing_double_quote(parent->str, i));
	}
	return (i);
}

int32_t	check_substitution_continuation(int32_t i, t_token *parent)
{
	i = goto_closing_parenthese(parent->str, i + 2);
	if (parent->str[i] != ')')
	{
		exec_delimiter_continuation(")", parent);
		return (goto_closing_parenthese(parent->str, i));
	}
	return (i);
}
