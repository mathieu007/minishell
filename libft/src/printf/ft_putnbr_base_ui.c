/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_u.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:24:57 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:24:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_base_ui(unsigned int n, char *base, int *write_len)
{
	unsigned int	base_len;

	if (*write_len == -1)
		return ;
	base_len = ft_strlen(base);
	if (n > base_len - 1)
	{
		ft_putnbr_base_ul(n / base_len, base, write_len);
		n %= base_len;
	}
	ft_putchar2(base[n], write_len);
}
