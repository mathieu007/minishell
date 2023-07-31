/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/31 13:15:32 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_parentheses_syntax_error(char *str)
{
	int32_t		i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] != '(')
	{
		get_process()->syntax_error = true;
		write_err(258, "syntax error near unexpected token `(");
		return (true);
	}
	while (str[i])
	{
		if (str[i] == ')')
		{
			while (str[i])
			{
				if (str[i] == ' ')
					i++;
				else if (str_is_redirection(&str[i]))
					break ;
				else
				{
					get_process()->syntax_error = true;
					write_err(258, "syntax error near unexpected token `)");
					return (true);
				}
			}
			break ;
		}
		i++;
	}
	return (false);
}

void	missing_closing_parenthese_error()
{
	char		*illegal_token;

	illegal_token = "syntax error misssing clossing parenthese";
	get_process()->syntax_error = true;
	write_err(258, illegal_token);
}
