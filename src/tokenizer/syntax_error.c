/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/31 20:32:00 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_open_parenthese_error(char *str)
{
	int32_t	i;

	i = 0;
	i = goto_closing_parenthese(str, i + 1);
	if (str[i] != ')')
	{
		write_syntax_error("syntax error near unexpected token `('\n");
		return (true);
	}
	i++;
	while (str[i] && str[i] == ' ')
		i++;
	if (!str[i] || str_is_redirection(&str[i]))
		return (false);
	else if (str[i] == '(')
		write_syntax_error("syntax error near unexpected token `('\n");
	else if (str[i] == ')')
		write_syntax_error("syntax error near unexpected token `)'\n");
	else
		write_syntax_error2("syntax error near unexpected token `", &str[i]);
	return (true);
}

bool	srch_error_parenthese(char *str)
{
	int32_t	i;
	char	*srch;

	i = 0;
	srch = ft_strchr(&str[i], '(');
	if (srch)
		return (check_open_parenthese_error(&str[i]));
	else
	{
		srch = ft_strchr(&str[i], ')');
		if (srch)
		{
			write_syntax_error("syntax error near unexpected token `)'\n");
			return (true);
		}
	}
	return (false);
}

bool	check_parentheses_syntax_error(char *str)
{
	int32_t	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == ')')
	{
		write_syntax_error("syntax error near unexpected token `)'\n");
		return (true);
	}
	else if (str[i] == '(')
		return (check_open_parenthese_error(&str[i]));
	else
		return (srch_error_parenthese(&str[i]));
	return (false);
}

void	missing_closing_parenthese_error(void)
{
	char	*illegal_token;

	illegal_token = "syntax error misssing clossing parenthese";
	get_process()->syntax_error = true;
	write_err(258, illegal_token);
}
