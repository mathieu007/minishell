/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strequal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:47:31 by mroy              #+#    #+#             */
/*   Updated: 2023/07/19 15:37:56 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_strequal(char *str, char *str2)
{
	if (!str || !str)
		return (false);
	if (ft_strlen(str) != ft_strlen(str2))
		return (false);
	while (*str)
	{
		if (*str != *str2)
			return (false);
		str++;
		str2++;
	}
	return (true);
}
