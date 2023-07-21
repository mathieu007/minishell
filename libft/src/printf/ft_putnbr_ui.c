/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_u.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:25:28 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:25:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_ui(unsigned int n, int *write_len)
{
	if (*write_len == -1)
		return ;
	if (n > 9)
	{
		ft_putnbr2(n / 10, write_len);
		n %= 10;
	}
	ft_putchar2(n + '0', write_len);
}
