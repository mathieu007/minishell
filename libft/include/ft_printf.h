/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/11/17 10:55:45 by mroy              #+#    #+#             */
/*   Updated: 2022/11/17 10:55:45 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# include <stdio.h>

size_t	ft_strlen2(char *str2);
void	ft_putchar2(char c, int *write_len);
void	ft_putstr2(char *s, int *write_len);
void	ft_putnbr2(int n, int *write_len);
void	ft_putnbr_ui(unsigned int n, int *write_len);
void	ft_putnbr_base_ui(unsigned int n, char *base, int *write_len);
void	ft_putnbr_base_ul(unsigned long n, char *base, int *write_len);
void	ft_putnbr_base(int n, char *base, int *write_len);
int		ft_printf(const char *str, ...);
void	print_hex(va_list arg, int *write_len, char c);
void	print_addr_hex(va_list arg, int *write_len);
void	print_char(va_list arg, int *write_len);
void	print_int(va_list arg, int *write_len);
void	print_uint(va_list arg, int *write_len);
void	print_str(va_list arg, int *write_len);

#endif