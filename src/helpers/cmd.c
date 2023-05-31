/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/05/31 13:43:04 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_seq	get_sequence_type(t_token_type type)
{
	if (type == TK_AMPERSAND || type == TK_GREATGREAT || type == TK_GREAT
		|| type == TK_LESS || type == TK_LESSLESS || type == TK_LAST_PIPE_EXIT
		|| type == TK_PIPE || type == TK_AND || type == TK_OR)
		return ((t_cmd_seq)((int32_t)type));
	return (CMD_SEQUENTIAL);
}

bool	is_builtins(char *str)
{
	int32_t	i;
	char	**builtins;
	int32_t	len;

	builtins = get_builtins_cmd();
	i = 0;
	while (builtins[i])
	{
		len = ft_strlen(builtins[i]);
		if (ft_strncmp(str, builtins[i], len) == 0)
			return (true);
		i++;
	}
	return (false);
}

/// @brief determine the end of a sequence...
/// @param token 
/// @return 
inline bool	is_end_of_seq(t_token *token)
{
	t_token_type	type;

	type = token->type;
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE
		|| type == TK_CMD_SEQ_END)
		return (true);
	return (false);
}
