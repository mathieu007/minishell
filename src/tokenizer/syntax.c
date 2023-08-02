/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/02 11:13:11 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_syn_errors2(char *str, int32_t i)
{
	if (str[i] == '(' && str[i + 1] == ')')
		write_syntax_error("syntax error near unexpected token `('\n");
	else if (str[i] == '(')
		write_syntax_error("syntax error near unexpected token `('\n");
	else if (str[i] == ')')
		write_syntax_error("syntax error near unexpected token `)'\n");
	else
		write_syntax_error2("syntax error near unexpected token `", &str[i]);
}
