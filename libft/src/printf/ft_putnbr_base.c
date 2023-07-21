/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:24:41 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:24:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_base(int n, char *base, int *write_len)
{
	unsigned int	number;
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
		ft_putnbr_base(number / base_len, base, write_len);
		number %= base_len;
	}
	ft_putchar2(base[number], write_len);
}
