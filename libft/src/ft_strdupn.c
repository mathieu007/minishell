/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdupn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:47:31 by mroy              #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/05/09 13:10:53 by bmartin          ###   ########.fr       */
=======
/*   Updated: 2023/05/09 13:11:01 by mroy             ###   ########.fr       */
>>>>>>> origin/Math
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	if (!dest || !src)
		return (NULL);
	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdupn(const char *s1, size_t n)
{
<<<<<<< HEAD
	char	*dest;
=======
	char		*dest;
>>>>>>> origin/Math
	size_t		src_l;

	src_l = ft_strlen(s1);
	if (n > src_l)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * (n + 1));
	if (!dest)
		return (NULL);
	dest = ft_strcpy(dest, s1);
	return (dest);
}

