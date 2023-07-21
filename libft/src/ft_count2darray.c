/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count2darray.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 12:57:53 by mroy              #+#    #+#             */
/*   Updated: 2023/07/21 10:11:09 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_count2darray(const char **arr)
{
	size_t	count;

	count = 0;
	if (!arr)
		return (count);
	while (*arr)
	{
		arr++;
		count++;
	}
	return (count);
}
