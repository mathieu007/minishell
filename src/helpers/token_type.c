/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/07/21 15:32:21 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (i);
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
	return (i);
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
		return (i);
	return (i - 1);
}

inline int32_t	goto_closing_parenthese(char *str, int32_t i)
{
	while (str[i])
	{
		if (str[i] == '\\')
			i += 1;
		else if (str[i] == '"')
			i = goto_closing_double_quote(str, i + 1);
		else if (str[i] == '\'')
			i = goto_closing_single_quote(str, i + 1);
		else if (str[i] == '(')
			i = goto_closing_parenthese(str, i + 1);
		else if (str[i] == '$' && str[i] && str[i] == '{')
			i = goto_closing_environement(str, i + 2);
		else if (str[i] == ')')
			return (i);
		if (str[i] != '\0')
			i++;
	}
	return (i);
}

inline int32_t	get_token_len(char *str, t_token_type type, bool in_quotes)
{
	if (type == TK_UNKNOWN)
		return (1);
	else if (str[0] == '\0')
		return (0);
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
 