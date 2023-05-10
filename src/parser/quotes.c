/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 09:33:21 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief single quote str is a string literal
char	*cpy_single_quote_str(char *input, char *output, int32_t *i)
{
	while (*input)
	{
		*output++ = input[*i];
		if (is_closing_single_quote(input, *i))
		{
			(*i)++;
			break ;
		}				
		(*i)++;
	}
	return (output);
}
