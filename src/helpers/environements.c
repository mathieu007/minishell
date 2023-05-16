/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 08:33:12 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	is_esc_env_var(char *str, int32_t i)
{
	if (!str || !str[i])
		return (false);
	return (str[i] == '\\' && str[i + 1] != '\0' && str[i + 1] == '$');
}

/// @brief this function assusme that preceding char is not an escaped char
/// use is_escaped_env_variable(str ,i) before
/// @param str 
/// @param i 
/// @return 
inline bool	is_env_variable(t_token *token)
{
	if (!token || !token->str)
		return (false);
	return (token->str[0] == '$' && ft_isalpha(token->str[1]) == 1);
}

/// @brief this function assusme that preceding char is not an escaped char
/// use is_escaped_env_variable(str ,i) before
/// @param str 
/// @param i 
/// @return 
inline bool	str_is_env_variable(char *str)
{
	if (!str || !str[0])
		return (false);
	return (str[0] == '$' && ft_isalpha(str[1]) == 1);
}
