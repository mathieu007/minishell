/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_is.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/04/30 08:36:54 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief check if type is a single quote \'
/// @param tokens 
/// @return 
bool	is_escaped_single_quote(t_token *tokens)
{
	return (tokens->type == TK_BACKSLASH && tokens->next != NULL
		&& tokens->next->value == '\'');
}

/// @brief check if token is an escaped double quote: \"
/// @param tokens 
/// @return 
bool	is_escaped_double_quote(t_token *tokens)
{
	return (tokens->type == TK_BACKSLASH && tokens->next != NULL
		&& tokens->next->value == '\"');
}
