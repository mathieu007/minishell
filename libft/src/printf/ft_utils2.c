/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:27:54 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:27:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_str(va_list arg, int *write_len)
{
	char	*str;

	if (*write_len == -1)
		return ;
	str = va_arg(arg, void *);
	if (str == NULL)
		ft_putstr2("(null)", write_len);
	else
		ft_putstr2(str, write_len);
}

void	print_char(va_list arg, int *write_len)
{
	char	c;

	if (*write_len == -1)
		return ;
	c = va_arg(arg, int);
	*write_len += write(1, &c, 1);
}
