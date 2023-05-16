/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_counter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/05/12 10:05:43 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


inline int32_t	*get_token_counter(void)
{
	static int32_t	counter[255 * 255];

	return (&counter[0]);
}

int32_t	*reset_token_counter(void)
{
	int32_t	*counter;
	int32_t	i;
	int32_t	len;

	i = 0;
	len = 255 * 255;
	counter = get_token_counter();
	while (i < len)
	{
		counter[i] = 0;
		i++;
	}
	return (counter);
}

inline int32_t	get_token_type_count(t_token_type type)
{
	int32_t	*counter;

	if (type == TK_UNKNOWN)
		return (0);
	counter = get_token_counter();
	return (counter[(int32_t) type]);
}

inline int32_t	increment_counter(t_token_type type)
{
	int32_t	*counter;

	if (type == TK_UNKNOWN)
		return (0);
	counter = get_token_counter();
	counter[(int32_t) type]++;
	return (counter[(int32_t) type]);
}

inline int32_t	decrement_counter(t_token_type type)
{
	int32_t	*counter;

	if (type == TK_UNKNOWN)
		return (0);
	counter = get_token_counter();
	counter[(int32_t) type]--;
	return (counter[(int32_t) type]);
}
