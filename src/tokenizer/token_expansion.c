/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 12:03:25 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int32_t	goto_closing_expansion_token(char *str, t_token_type type,
		int32_t i)
{
	if (type == TK_COMMANDSUBSTITUTION_OPEN)
		i = goto_closing_parenthese(str, i + 2);
	else if (type == TK_ENVIRONEMENT_VAR)
		i = goto_closing_environement(str, i + 1);
	else if (type == TK_DOLLAR_SIGN_CURLYBRACE)
		i = goto_closing_environement(str, i + 2);
	else if (type == TK_LAST_CMD_EXIT)
		i++;
	return (i);
}

int32_t	add_expansion_token(char *str, int32_t i, char *tk_str, t_token *parent)
{
	t_token_type	type;

	if (!str[i])
		return (i);
	type = get_token_type(tk_str);
	add_tk(tk_str, type, i, parent);
	i = goto_closing_expansion_token(str, type, i);
	if (!str[i])
		return (i);
	type = get_token_type(&str[i]);
	if (str[i] == '}')
		add_tk("}", type, i, parent);
	else if (str[i] == ')')
		add_tk(")", type, i, parent);
	else
		add_tk("", type, ++i, parent);
	return (i);
}

void	split_token_expansion(t_token *parent)
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
		token->child = NULL;
		token = token->next;
	}
}

int32_t	process_expansion_token(int32_t i, char *str, t_token *parent)
{
	t_token_type	type;
	int32_t			t_len;

	type = get_token_type(&str[i]);
	t_len = get_token_len(&str[i], type);
	if (type == TK_SINGLEQUOTE)
		return (skip_token_single_quote(str, type, i));
	else if (type == TK_COMMANDSUBSTITUTION_OPEN)
		return (add_expansion_token(str, i, "$(", parent));
	else if (type == TK_DOLLAR_SIGN_CURLYBRACE)
		return (add_expansion_token(str, i, "${", parent));
	else if (type == TK_ENVIRONEMENT_VAR)
		return (add_expansion_token(str, i, "$", parent));
	else if (type == TK_LAST_CMD_EXIT)
		return (add_expansion_token(str, i, "$?", parent));
	else
		return (i + t_len);
}

t_token	*expansion_tokenizer(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	char			*str;

	i = 0;
	str = parent->str;
	if (!has_token_expansion_str(str))
		return (NULL);
	type = TK_START;
	if (!is_token_redirection(parent->type))
		type = TK_CMD;
	add_tk("", type, i, parent);
	while (str[i])
	{
		if (has_error())
			return (parent->child);
		i = process_expansion_token(i, str, parent);
		if (i == -1)
			return (parent->child);
	}
	add_tk("", TK_END, i, parent);
	split_token_expansion(parent);
	return (parent->child);
}

// t_token	*expansion_tokenizer(t_token *parent)
// {
// 	int32_t			i;
// 	t_token_type	type;
// 	int32_t			t_len;
// 	char			*str;

// 	str = parent->str;
// 	i = 0;
// 	if (!has_token_expansion_str(str))
// 		return (NULL);
// 	type = TK_START;
// 	if (!is_token_redirection(parent->type))
// 		type = TK_CMD;
// 	add_tk("", type, i, parent);
// 	while (str[i])
// 	{
// 		if (has_error())
// 			return (parent->child);
// 		type = get_token_type(&str[i]);
// 		t_len = get_token_len(&str[i], type);
// 		if (type == TK_SINGLEQUOTE)
// 			i = skip_token_single_quote(str, type, i);
// 		else if (type == TK_COMMANDSUBSTITUTION_OPEN)
// 			i = add_expansion_token(str, i, "$(", parent);
// 		else if (type == TK_DOLLAR_SIGN_CURLYBRACE)
// 			i = add_expansion_token(str, i, "${", parent);
// 		else if (type == TK_ENVIRONEMENT_VAR)
// 			i = add_expansion_token(str, i, "$", parent);
// 		else if (type == TK_LAST_CMD_EXIT)
// 			i = add_expansion_token(str, i, "$?", parent);
// 		else
// 			i += t_len;
// 	}
// 	add_tk("", TK_END, i, parent);
// 	split_token_expansion(parent);
// 	return (parent->child);
// }
