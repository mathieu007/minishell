/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/14 09:54:21 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief check if str is an escaped char
/// @param tokens 
/// @return 
inline int32_t	skip_escaped_char(char *str, int32_t i)
{
	if (!str || !str[i])
		return (i);
	if (str[i] == '\\')
		return (i + 2);
	return (i);
}

/// @brief check if str is an escaped char and return the str after.
/// @param tokens 
/// @return 
inline char	*skip_escaped_char2(char *str)
{
	if (!str || !str[0])
		return (str);
	if (*str == '\\')
		return (&str[2]);
	return (str);
}
