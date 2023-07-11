/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/07/08 09:36:38 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	is_semicolon(t_token_type type)
{
	if (type == TK_SEMICOLON)
		return (true);
	return (false);
}

inline bool	is_end_of_seq(t_token_type type)
{
	if (type == TK_AND || type == TK_OR || type == TK_PIPE)
		return (true);
	return (false);
}

inline int32_t	goto_closing_single_quote(char *str, int32_t i)
{
	while (str[i])
	{
		if (str[i] == '\\')
			i += 2;
		else if (str[i] == '\'')
			return (i);
		else
			i++;
	}
	return (i - 1);
}

inline int32_t	goto_closing_double_quote(char *str, int32_t i)
{
	while (str[i])
	{
		if (str[i] == '\\')
			i += 2;
		else if (str[i] == '"')
			return (i);
		else
			i++;
	}
	return (i - 1);
}

inline int32_t	goto_closing_environement(char *str, int32_t i)
{
	if (str[i] == '{')
		i++;
	if (ft_isnum(&str[i]))
	{
		i++;
		if (str[i] == '}')
			i++;
		return (i - 1);
	}		
	while (ft_isalnum(str[i]) == 1 || str[i] == '_')
		i++;
	if (str[i] == '}')
		i++;
	return (i - 1);
}

inline int32_t	goto_closing_parenthese(char *str, int32_t i)
{
	while (str[i])
	{
		if (str[i] == '\\')
			i += 2;
		else if (str[i] == '"')
			i = goto_closing_double_quote(str, i + 1) + 1;
		else if (str[i] == '\'')
			i = goto_closing_single_quote(str, i + 1) + 1;
		else if (str[i] == '(')
			i = goto_closing_parenthese(str, i + 1);
		else if (str[i] == ')')
			return (i);
		else
			i++;
	}
	return (i - 1);
}


inline int32_t	get_token_len(char *str, t_token_type type, bool in_quotes)
{
	if (type == TK_UNKNOWN)
		return (1);
	else if (type == TK_DASH && !in_quotes)
		return (get_token_dash_len(str));
	else if (type == TK_DASHDASH && !in_quotes)
		return (get_token_dashdash_len(str));
	else if (type == TK_ENVIRONEMENT_VAR)
		return (get_token_env_len(str));
	if ((int32_t)type > 255 && type != TK_UNKNOWN)
		return (2);
	return (1);
}

inline int32_t	get_token_type_len2(t_token_type type)
{
	if (type == TK_UNKNOWN || type == TK_END
		|| type == TK_START)
		return (0);
	if ((int32_t)type > 255)
		return (2);
	return (1);
}
