/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/06/12 10:07:49 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



bool	is_builtins(char *str)
{
	int32_t	i;
	char	**builtins;
	size_t	len;

	builtins = get_builtins_cmd();
	i = 0;
	while (builtins[i])
	{
		len = ft_strlen(builtins[i]);
		if (ft_strncmp(str, builtins[i], len) == 0 && len == ft_strlen(str))
			return (true);
		i++;
	}
	return (false);
}

/// @brief determine the end of a sequence...
/// @param token 
/// @return 
inline bool	is_token_sequence(t_token *token)
{
	t_token_type	type;

	type = token->type;
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE)
		return (true);
	return (false);
}
