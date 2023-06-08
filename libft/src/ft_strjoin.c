/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 20:16:34 by mathieu           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/06/01 11:53:19 by bmartin          ###   ########.fr       */
=======
/*   Updated: 2023/06/05 14:56:34 by math             ###   ########.fr       */
>>>>>>> origin/Math
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*mem;
	size_t	s1_l;
	size_t	s2_l;

	s1_l = ft_strlen(s1);
	s2_l = ft_strlen(s2);
	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
	if (!mem)
		return (NULL);
	ft_memcpy(mem, s1, s1_l);
	ft_memcpy(&mem[s1_l], s2, s2_l);
	mem[s1_l + s2_l] = '\0';
	return (mem);
}

char	*ft_strjoinfree(char *s1, char const *s2)
{
	char	*mem;
	size_t	s1_l;
	size_t	s2_l;

	s1_l = ft_strlen(s1);
	s2_l = ft_strlen(s2);
	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
	if (!mem)
		return (free(s1), NULL);
	ft_memcpy(mem, s1, s1_l);
	ft_memcpy(&mem[s1_l], s2, s2_l);
	mem[s1_l + s2_l] = '\0';
	return (free(s1), mem);
<<<<<<< HEAD
}
=======
}

char	*ft_strjoinfree2(char *s1, char *s2)
{
	char	*mem;
	size_t	s1_l;
	size_t	s2_l;

	s1_l = ft_strlen(s1);
	s2_l = ft_strlen(s2);
	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
	if (!mem)
	{
		if (s1)
			free(s1);
		if (s2)
			free(s2);
		return (NULL);
	}
	ft_memcpy(mem, s1, s1_l);
	ft_memcpy(&mem[s1_l], s2, s2_l);
	mem[s1_l + s2_l] = '\0';
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (mem);
}
>>>>>>> origin/Math
