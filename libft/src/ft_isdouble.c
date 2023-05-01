/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdouble.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 12:57:53 by mroy              #+#    #+#             */
/*   Updated: 2023/04/25 11:16:11 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_isdouble(char *str)
{
	if (str == NULL)
		return (false);
	if (*str && (*str == '-' || *str == '+'))
		str++;
	while (*str && (*str >= '0' && *str <= '9'))
		str++;
	if (*str == '.')
		str++;
	while (*str && (*str >= '0' && *str <= '9'))
		str++;
	if (*str == '\0')
		return (true);
	return (false);
}
