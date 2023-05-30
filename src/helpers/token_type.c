/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/05/29 16:51:02 by math             ###   ########.fr       */
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

	if (type == TK_UNKNOWN || type == TK_CMD_SEQ_END
		|| type == TK_CMD_SEQ_START)
		return (0);
	if ((int32_t)type > 255)
		return (2);
	return (1);
}
