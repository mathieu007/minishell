/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/05/17 10:56:37 by mroy             ###   ########.fr       */
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
	if (type == TK_UNKNOWN || type == TK_CMD_SEQ_END || type == TK_CMD_SEQ_START)
		return (0);
	if ((int32_t)type > 255)
		return (2);
	return (1);
}
