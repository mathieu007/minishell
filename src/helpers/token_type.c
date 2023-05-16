/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/05/15 15:57:14 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	type_is_end_of_seq(t_token_type type)
{
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE
		|| type == TK_CMD_SEQ_END)
		return (true);
	return (false);
}

inline int32_t	get_token_type_len(t_token_type type)
{
	if ((int32_t)type > 255)
		return (2);
	else if ((int32_t)type > 255 * 255 + 255)
		return (0);
	return (1);
}
