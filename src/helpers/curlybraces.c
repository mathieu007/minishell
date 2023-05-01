/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curlybraces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/01 13:31:19 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	is_opening_curlybrace(char *str)
{
	if (!str || !*str)
		return (false);
	return (*str == '{' && get_token_type_count(TK_CURLYBRACES_OPEN) % 2 == 0);
}

inline bool	is_closing_curlybrace(char *str)
{
	if (!str || !*str)
		return (false);
	return (*str == '}' && get_token_type_count(TK_CURLYBRACES_CLOSE) % 2 == 1);
}
