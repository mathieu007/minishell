/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/05/09 10:58:00 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	*get_tokens_lookup_table(void)
{
	static t_token_type	token_types[255 * 255];
	static int32_t		i;

	if (i == 0)
	{
		while (i < 255 * 255)
		{
			token_types[i] = (t_token_type)i;
			i++;
		}
	}
	return (&token_types[0]);
}

t_token_type	get_token_type(char *str)
{
	t_token_type	*lookup_table;

	if (!str || *str == '\0')
		return (TK_UNKNOWN);
	if (*str == '\0')
		return (TK_END);
	lookup_table = get_tokens_lookup_table();
	if (str[1] != '\0' && lookup_table[str[0] * str[1] + str[1]] != TK_UNKNOWN)
		return (lookup_table[str[0] * str[1] + str[1]]);
	return (lookup_table[(int32_t)str[0]]);
}
