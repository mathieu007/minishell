/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   esc_quote.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 14:09:07 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief check if str is an escaped single quote: \'
/// @param tokens 
/// @return 
// inline bool	is_escaped_single_quote(char *str, int32_t i, t_token_group *group)
// {
// 	if (!str || !str[i] || !str[i + 1])
// 		return (false);
// 	return (str[i] == '\\' && str[i + 1] == '\'');
// }

/// @brief check if str is an escaped double quote: \"
/// @param tokens 
/// @return 
// inline bool	is_escaped_double_quote(char *str, int32_t i, t_token_group *group)
// {
// 	if (!str || !str[i] || !str[i + 1])
// 		return (false);
// 	return (str[i] == '\\' && str[i + 1] == '\"');
// }
