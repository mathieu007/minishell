/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/05 13:24:29 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// inline bool	is_opening_single_quote(char *str, int32_t i)
// {
// 	if (!str || !str[i])
// 		return (false);
// 	return (str[i] == '\''
// 		&& get_token_type_count(TK_SINGLEQUOTE) % 2 == 0);
// }

// inline bool	is_closing_single_quote(char *str, int32_t i)
// {
// 	if (!str || !str[i])
// 		return (false);
// 	return (str[i] == '\''
// 		&& get_token_type_count(TK_SINGLEQUOTE) % 2 == 1);
// }

// inline bool	is_opening_double_quote(char *str, int32_t i)
// {
// 	if (!str || !str[i])
// 		return (false);
// 	return (str[i] == '"'
// 		&& get_token_type_count(TK_DOUBLEQUOTE) % 2 == 0);
// }

// inline bool	is_closing_double_quote(char *str, int32_t i)
// {
// 	if (!str || !str[i])
// 		return (false);
// 	return (str[i] == '"'
// 		&& get_token_type_count(TK_DOUBLEQUOTE) % 2 == 1);
// }
