/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 09:58:43 by mroy             ###   ########.fr       */
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
inline bool	is_env_variable(char *str, int32_t i)
{
	if (!str || !str[i])
		return (false);
	return (str[i] == '$');
}

