/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escaped_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 09:39:01 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief copy escaped environement variable as str literal.
char	*cpy_esc_env_var(char *input, char *output, int32_t *i)
{	
	if (!input[*i])
		return (output);
	*output++ = input[(*i)++];
	*output++ = input[(*i)++];
	return (output);
}
