/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/31 20:27:27 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_syntax_error(char *msg)
{
	get_process()->syntax_error = true;
	write_err(258, msg);
}

void	write_syntax_error2(char *msg, char *msg2)
{
	get_process()->syntax_error = true;
	write_err2(258, msg, msg2);
}

void	write_syntax_error3(char *msg, char *msg2, char *msg3)
{
	get_process()->syntax_error = true;
	write_err3(258, msg, msg2, msg3);
}
