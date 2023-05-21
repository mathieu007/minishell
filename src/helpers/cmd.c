/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/05/21 12:10:12 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_seq	get_sequence_type(t_token_type type)
{
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE)
		return ((t_cmd_seq)type);
	return (CMD_SEQUENTIAL);
}

/// @brief simply get the end of a cmd sequence.
/// @param str 
/// @return 
char	*get_end_of_cmd(char *str)
{
	t_token_type	type;
	int32_t 		i;
	
	i = 0;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		if (type_is_end_of_seq(type))
			return (&str[i + get_token_type_len(type)]);
		i++;
	}
	return (&str[i]);
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
		if (ft_strn_right_cmp(str, builtins[i], len))
			return (true);
		i++;
	}
	return (false);
}

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
