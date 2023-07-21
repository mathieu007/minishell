/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:27:38 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:27:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_int(va_list arg, int *write_len)
{
	int	n;

	if (*write_len == -1)
		return ;
	n = va_arg(arg, int);
	ft_putnbr(n, write_len);
}

void	print_uint(va_list arg, int *write_len)
{
	unsigned int	n;

	if (*write_len == -1)
		return ;
	n = va_arg(arg, unsigned int);
	ft_putnbr_ui(n, write_len);
}

void	print_hex(va_list arg, int *write_len, char c)
{
	unsigned int	n;

	if (*write_len == -1)
		return ;
	n = va_arg(arg, unsigned int);
	if (c == 'X')
		ft_putnbr_base_ui(n, "0123456789ABCDEF", write_len);
	else
		ft_putnbr_base_ui(n, "0123456789abcdef", write_len);
}

void	print_addr_hex(va_list arg, int *write_len)
{
	unsigned long	addr;

	if (*write_len == -1)
		return ;
	addr = va_arg(arg, unsigned long);
	if (addr == 0)
		ft_putstr2("0x0", write_len);
	else
	{
		ft_putstr2("0x", write_len);
		ft_putnbr_base_ul(addr, "0123456789abcdef", write_len);
	}
}
