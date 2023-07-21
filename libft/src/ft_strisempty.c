/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strisempty.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 12:57:53 by mroy              #+#    #+#             */
/*   Updated: 2023/07/11 09:50:08 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_strisempty(char *str)
{
	if (!str)
		return (true);
	if (str[0] == '\0')
		return (true);
	return (false);
}

bool	ft_striswhitespace(char *str)
{
	if (!str)
		return (true);
	while (*str == ' ')
		str++;
	if (*str == '\0')
		return (true);
	return (false);
}
