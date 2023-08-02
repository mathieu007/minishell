/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/02 11:15:10 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	advance_to_end_of_quotes(char *str, int32_t i)
{
	if (!str[i])
		return (i);
	if (str[i] == '"')
	{
		i = goto_closing_double_quote(str, i + 1);
		if (str[i])
			i++;
	}
	if (str[i] == '\'')
	{
		i = goto_closing_single_quote(str, i + 1);
		if (str[i])
			i++;
	}
	return (i);
}

bool	check_open_parenthese_error(char *str)
{
	int32_t	i;

	i = 0;
	i = advance_to_end_of_quotes(str, i);
	i = goto_closing_parenthese(str, i + 1);
	if (str[i] != ')' || (str[i] == ')' && i > 0 && str[i - 1] == '('))
	{
		write_syntax_error("syntax error near unexpected token `('\n");
		return (true);
	}
	i++;
	while (str[i] && str[i] == ' ')
		i++;
	if (!str[i] || str_is_redirection(&str[i]))
		return (false);
	write_syn_errors2(str, i);
	return (true);
}

bool	srch_error_parenthese(char *str)
{
	int32_t	i;

	i = 0;
	while (str[i])
	{
		i = advance_to_end_of_quotes(str, i);
		if (str[i] == '(' && str[i + 1] == ')')
		{
			write_syntax_error("syntax error near unexpected token `('\n");
			return (true);
		}
		else if (str[i] == '(')
			return (check_open_parenthese_error(&str[i]));
		else if (str[i] == ')')
		{
			write_syntax_error("syntax error near unexpected token `)'\n");
			return (true);
		}
		i++;
	}
	return (false);
}

bool	check_parentheses_syntax_error(char *str)
{
	int32_t	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	i = advance_to_end_of_quotes(str, i);
	if (str[i] == ')')
	{
		write_syntax_error("syntax error near unexpected token `)'\n");
		return (true);
	}
	else if (str[i] == '(' && str[i + 1] == ')')
	{
		write_syntax_error("syntax error near unexpected token `('\n");
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
