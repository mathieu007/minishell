/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:25:38 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:25:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr2(int n, int *write_len)
{
	unsigned int	num;

	if (*write_len == -1)
		return ;
	if (n < 0)
	{
		ft_putchar2('-', write_len);
		num = -n;
	}
	else
		num = n;
	if (num > 9)
	{
		ft_putnbr2(num / 10, write_len);
		num %= 10;
	}
	ft_putchar(num + '0', write_len);
}
