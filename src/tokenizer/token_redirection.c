/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/11 10:13:02 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_newline_syntax_error(char *str)
{
	char	*syntax_error;
	int32_t	i;

	i = 0;
	syntax_error = "syntax error near unexpected token: newline";
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == '\0')
	{
		get_process()->syntax_error = true;
		write_err(2, syntax_error);
		return (true);
	}
	return (false);
}

bool	check_syntax_error_near(char *str, char *token_err)
{
	int32_t	i;
	char	*illegal_token;

	illegal_token = "syntax error near unexpected token: ";
	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (*token_err)
	{
		if (*token_err == str[i])
		{
			get_process()->syntax_error = true;
			write_err2(2, illegal_token, &str[i]);
			return (true);
		}
		token_err++;
	}
	return (false);
}

bool	has_syntax_errors(char *str, int32_t i, int32_t len)
{
	if (check_newline_syntax_error(&str[i + len])
		|| check_syntax_error_near(&str[i + len], "<>|&;()#"))
		return (true);
	return (false);
}

int32_t	add_token_redirection(char *str, int32_t i, t_token_type type,
		t_token *parent)
{
	t_token	*token;
	int32_t	len;

	len = get_token_len(&str[i], type, false);
	if (has_syntax_errors(str, i, len))
		return (i);
	token = add_token(i, type, parent);
	token->token_len = len;
	token->token_str = ft_substr(str, i, token->token_len);
	if (!token->token_str)
		free_all_and_exit2(errno, "malloc error");
	token->end = i + token->token_len;
	i += token->token_len;
	while (str[i] && str[i] == ' ')
		i++;
	return (i);
}

void	split_token_redirections(t_token *parent)
{
	char	*str;
	int32_t	start;
	int32_t	len;
	t_token	*token;

	token = parent->child;
	str = parent->str;
	while (token && token->next)
	{
		len = token->next->start - token->end;
		start = token->start + token->token_len;
		token->str = free_ptr(token->str);
		token->str = ft_substr(str, start, len);
		token->child = expansion_tokenizer(parent);
		token = token->next;
	}
}

t_token	*redirection_tokenizer(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	i = 0;
	if (!has_token_redirection(parent))
		return (NULL);
	add_tk("", TK_START, i, parent);
	while (parent->str[i])
	{
		if (has_error())
			return (parent->child);
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (is_token_redirection(type))
			i = add_token_redirection(parent->str, i, type, parent);
		else
			i += t_len;
	}
	add_tk("", TK_END, i, parent);
	split_token_redirections(parent);
	return (parent->child);
}
