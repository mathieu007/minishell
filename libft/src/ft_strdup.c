/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:47:31 by mroy              #+#    #+#             */
/*   Updated: 2023/07/21 08:28:48 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dest;
	int		src_l;

	if (!s1)
		return (ft_strdup(""));
	src_l = ft_strlen(s1);
	dest = malloc(sizeof(char) * (src_l + 1));
	dest[src_l] = '\0';
	if (!dest)
		return (NULL);
	dest = ft_strcpy(dest, s1);
	if (!dest)
		return (free(dest), NULL);
	return (dest);
}
