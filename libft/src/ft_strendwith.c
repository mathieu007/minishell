/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strendwith.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:47:31 by mroy              #+#    #+#             */
/*   Updated: 2023/07/21 11:35:21 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_strendwith(char *src, char *end_with)
{
	int32_t	end_with_len;
	int32_t	src_len;
	int32_t	i;

	if (!src || !end_with)
		return (false);
	end_with_len = ft_strlen(end_with);
	src_len = ft_strlen(src);
	if (src_len < end_with_len)
		return (false);
	src = &src[src_len - end_with_len];
	i = 0;
	while (src[i] && end_with[i])
	{
		if (src[i] != end_with[i])
			break ;
		i++;
	}
	if (src[i] != '\0' && end_with[i] != '\0')
		return (false);
	return (true);
}
