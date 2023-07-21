/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/10/29 15:25:08 by marvin            #+#    #+#             */
/*   Updated: 2022/10/29 15:25:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar2(char c, int *write_len)
{
	int	written;

	if (*write_len == -1)
		return ;
	written = 0;
	written = write(1, &c, 1);
	if (written == -1)
		*write_len = -1;
	else
		*write_len += written;
}
