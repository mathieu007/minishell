/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 20:16:34 by mathieu           #+#    #+#             */
/*   Updated: 2022/10/19 08:29:43 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_strn_right_cmp(const char *s1, const char *s2, size_t n)
{
	uint32_t	i;
	uint32_t	s1_len;
	uint32_t	s2_len;

	i = 0;
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s1_len == 0 || s2_len == 0)
		return (false);
	if (s1_len < n || s2_len < n)
		return (false);
	s1_len--;
	s2_len--;
	while (i < n && s1[s1_len] == s2[s2_len])
		i++;
	if (i != n)
		return (true);
	return (false);
}
