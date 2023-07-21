/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:25:47 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:25:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putstr2(char *s, int *write_len)
{
	int	i;

	if (*write_len == -1)
		return ;
	i = 0;
	while (s[i])
	{
		ft_putchar2(s[i], write_len);
		if (*write_len == -1)
			return ;
		i++;
	}
}
