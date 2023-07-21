/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstartwith.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:47:31 by mroy              #+#    #+#             */
/*   Updated: 2023/07/21 10:49:28 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_strstartwith(char *src, char *start)
{
	int32_t	i;

	if (!src || !start)
		return (false);
	i = 0;
	while (src[i] && start[i])
	{
		if (src[i] != start[i])
			break ;
		i++;
	}
	if (start[i] != '\0')
		return (false);
	return (true);
}
