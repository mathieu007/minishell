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

void	ft_putnbr_base_l(long n, char *base, int *write_len)
{
	unsigned long	number;
	size_t			base_len;

	if (*write_len == -1)
		return ;
	base_len = ft_strlen(base);
	if (n < 0)
	{
		ft_putchar2('-', write_len);
		number = -n;
	}
	else
		number = n;
	if (number > base_len - 1)
	{
		ft_putnbr_base_l(number / base_len, base, write_len);
		number %= base_len;
	}
	ft_putchar2(base[number], write_len);
}
