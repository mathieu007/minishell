/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   esc_quote.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/12 12:38:58 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief check if str is an escaped quote: \'
/// @param tokens 
/// @return 
inline int32_t	skip_escaped_quotes(char *str, int32_t i)
{
	int32_t	start;

	if (!str)
		return (false);
	start = i;
	while (str[i] == '\\')
		i++;
	if (i - start > 0 && (str[i] == '\'' || str[i] == '\"'))
		return (i + 1);
	return (start);
}

/// @brief check if str is an escaped single quote: \'
/// @param tokens 
/// @return 
inline int32_t	skip_esc_single_quote(char *str, int32_t i)
{
	int32_t	start;

	if (!str)
		return (false);
	start = i;
	while (str[i] == '\\')
		i++;
	if (i - start > 0 && str[i] == '\'')
		return (i + 1);
	return (start);
}

/// @brief check if str is an escaped double quote: \"
/// @param tokens 
/// @return 
inline int32_t	skip_esc_double_quote(char *str, int32_t i)
{
	int32_t	start;

	if (!str)
		return (false);
	start = i;
	while (str[i] == '\\')
		i++;
	if (i - start > 0 && str[i] == '\"')
		return (i + 1);
	return (start);
}
