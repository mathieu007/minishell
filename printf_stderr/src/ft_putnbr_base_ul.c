/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_ul.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:14:31 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:14:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_base_ul(unsigned long n, char *base, int *write_len)
{
	unsigned long	b_l;

	if (*write_len == -1)
		return ;
	b_l = ft_strlen(base);
	if (n > b_l - 1)
	{
		ft_putnbr_base_ul(n / b_l, base, write_len);
		n %= b_l;
	}
	ft_putchar(base[n], write_len);
}
