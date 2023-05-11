/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:47:31 by mroy              #+#    #+#             */
/*   Updated: 2023/05/11 07:06:19 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *src, size_t len)
{
	size_t	i;
	char	*new;

	i = 0;
	if (len <= 0)
		return (NULL);
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	while (src[i] && i < len)
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
