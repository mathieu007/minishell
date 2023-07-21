/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcontains.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:47:31 by mroy              #+#    #+#             */
/*   Updated: 2023/07/21 09:47:32 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_strcontains(char *src, char *to_search)
{
	int32_t	i;
	int32_t	j;

	if (!src || !to_search)
		return (false);
	j = 0;
	while (src[j])
	{
		i = 0;
		while (to_search[i] && src[j + i] && to_search[i] == src[j + i])
			i++;
		if (to_search[i] == '\0')
			return (true);
		j++;
	}
	return (false);
}
