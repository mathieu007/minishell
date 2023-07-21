/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:26:15 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:26:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	print_format(char const *str, va_list arg, int *write_len)
{
	if (*str == 'c')
		ft_putchar2(va_arg(arg, int), write_len);
	else if (*str == '%')
		ft_putchar2(*str, write_len);
	else if (*str == 'd' || *str == 'i')
		print_int(arg, write_len);
	else if (*str == 'u')
		print_uint(arg, write_len);
	else if (*str == 'x' || *str == 'X')
		print_hex(arg, write_len, *str);
	else if (*str == 'p')
		print_addr_hex(arg, write_len);
	else if (*str == 's')
		print_str(arg, write_len);
	else
		ft_putchar2(*str, write_len);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		len;
	int		*write_len;

	va_start(args, str);
	len = 0;
	write_len = &len;
	while (*str)
	{
		if (*str == '%')
			print_format(&(*++str), args, write_len);
		else
			ft_putchar2(*str, write_len);
		if (*write_len == -1)
			return (-1);
		str++;
	}
	va_end(args);
	return (len);
}
