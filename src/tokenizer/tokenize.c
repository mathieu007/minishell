/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 08:54:21 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*add_tk(char *token_str, t_token_type type, int32_t i, t_token *parent)
{
	t_token	*token;

	token = add_token(i, type, parent);
	token->token_len = ft_strlen(token_str);
	token->token_str = free_ptr(token->token_str);
	token->token_str = ft_strdup(token_str);
	token->end = i + token->token_len;
	return (token);
}

static bool	is_any_of(char c, char *values)
{
	while (*values)
	{
		if (*values == c)
			return (true);
		values++;
	}
	return (false);
}

int32_t	check_newline_syntax_error(char *str, int32_t len, int32_t i)
{
	char	*syntax_error;

	syntax_error = "syntax error near unexpected token: newline";
	i += len;
	while (str[i] && str[i] == ' ')
		i++;
	if (!str[i])
	{
		get_process()->syntax_error = true;
		write_err(258, syntax_error);
		return (-1);
	}
	return (i);
}

int32_t	check_syntax_error_before_near(char *str, int32_t i, char *token_err)
{
	int32_t	start;
	char	*illegal_token;
	char	*c;

	start = i;
	illegal_token = "syntax error near unexpected token `";
	if (i > 0)
		i--;
	while (str[i] && str[i] == ' ')
		i--;
	if (is_any_of(str[i], token_err))
	{
		get_process()->syntax_error = true;
		c = ft_substr(str, start, 1);
		c = ft_strjoinfree(c, "'");
		write_err2(258, illegal_token, c);
		return (-1);
	}
	return (i);
}

int32_t	check_syntax_error_after_near(char *str, int32_t i, char *token_err)
{
	char		*illegal_token;
	char		*c;

	illegal_token = "syntax error near unexpected token `";
	while (str[i] && str[i] == ' ')
		i++;
	if (is_any_of(str[i], token_err))
	{
		get_process()->syntax_error = true;
		c = ft_substr(str, i, 1);
		c = ft_strjoinfree(c, "'");
		write_err2(258, illegal_token, c);
		free(c);
		return (-1);
	}
	return (i);
}
