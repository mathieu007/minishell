/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/01 13:31:10 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	is_opening_single_quote(char *str)
{
	if (!str || !*str)
		return (false);
	return (*str == '\'' && get_token_type_count(TK_SINGLEQUOTE) % 2 == 0);
}

inline bool	is_closing_single_quote(char *str)
{
	if (!str || !*str)
		return (false);
	return (*str == '\'' && get_token_type_count(TK_SINGLEQUOTE) % 2 == 1);
}

inline bool	is_opening_double_quote(char *str)
{
	if (!str || !*str)
		return (false);
	return (*str == '\"' && get_token_type_count(TK_DOUBLEQUOTE) % 2 == 0);
}

inline bool	is_closing_double_quote(char *str)
{
	if (!str || !*str)
		return (false);
	return (*str == '\"' && get_token_type_count(TK_DOUBLEQUOTE) % 2 == 1);
}
