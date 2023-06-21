/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/19 08:15:22 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	redirect_has_syntax_error(char *str)
{
	static char	illegal_token[100] = "syntax error near unexpected token \0";
	static char	token_err[100] = "<>|&;()#\0";
	int32_t		i;
	
	i = 0;
	while (str[i] == ' ')
		i++;
	str = &str[i];
	i = 0;
	while (illegal_token[i])
	{
		if (str[0] == illegal_token[i])
		{
			token_err[36] = str[0];
			token_err[37] = '\0';
			i = 0;
			while (illegal_token[i])
			{
				if (str[1] == illegal_token[i])
				{
					token_err[37] = str[1];
					token_err[38] = '\0';
					return (write_err(2, &token_err[0]), true);
				}
				i++;
			}
			return (write_err(2, &token_err[0]), true);
		}
		i++;
	}
	return (false);
}

int32_t	add_token_redir(char *str, int32_t i, t_token_type type,
		t_token *parent)
{
	t_token	*token;
	int32_t	len;

	len = get_token_len(&str[i], type, false);
	if (redirect_has_syntax_error(&str[i + len]))
		return (-1);
	token = add_token(i, type, parent);
	token->is_redirection = true;
	token->token_len = get_token_len(&str[i], type, false);
	token->token_str = ft_substr(&str[i], 0, token->token_len);
	if (!token->token_str)
		free_all_and_exit2(errno, "malloc error");
	token->end = i + token->token_len;
	i += token->token_len;
	while (str[i] && str[i] == ' ')
		i++;
	return (i);
}

/// @brief this is the second level of tokenization
/// this level of tokenization will tokenize all
/// the double quotes, single quotes, parentheses, substitution
/// and curlybrace but also spaces.
/// @param high
/// @return
t_token	*tokenize_redirection(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*str;
	t_token			*token;

	i = 0;
	str = ft_strdup(parent->str);
	token = add_tk(ft_strdup(parent->token_str), parent->type, i, parent);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (is_token_redir(type))
			i = add_token_redir(str, i, type, parent);
		else
			i += t_len;
		if (has_error())
			return (free(str), NULL);
	}
	add_tk(ft_strdup(""), TK_END, i, parent);
	free(str);
	parent->child = token;
	split_token_redirection(parent);
	return (parent->child);
}
