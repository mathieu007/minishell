/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_group2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/19 08:28:48 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	add_token_single_quote(char *str, int32_t i, t_token *parent)
{
	t_token	*token;

	token = add_tk(ft_strdup("'"), TK_SINGLEQUOTE, i, parent);
	i = goto_closing_single_quote(str, i + 1);
	if (str[i] != '\'')
		token->is_continuation = true;
	else
		add_tk(ft_strdup("\'"), TK_CLOSINGSINGLEQUOTE, i, parent);
	i++;
	return (i);
}

int32_t	add_token_double_quote(char *str, int32_t i, t_token *parent)
{
	t_token	*token;

	token = add_tk(ft_strdup("\""), TK_DOUBLEQUOTE, i, parent);
	i = goto_closing_double_quote(str, i + 1);
	if (str[i] != '"')
		token->is_continuation = true;
	else
		add_tk(ft_strdup("\""), TK_CLOSINGDOUBLEQUOTE, i, parent);
	i++;
	return (i);
}

int32_t	add_token_substitution(char *str, int32_t i, t_token *parent,
		bool inside_dbl_quotes)
{
	t_token	*token;

	token = add_tk(ft_strdup("$("), TK_COMMANDSUBSTITUTION_OPEN, i, parent);
	i = goto_closing_parenthese(str, i);
	if (str[i] != ')')
		token->is_continuation = true;
	else
		add_tk(ft_strdup(")"), TK_COMMANDSUBSTITUTION_CLOSE, i, parent);
	token->inside_dbl_quotes = inside_dbl_quotes;
	i++;
	return (i);
}

int32_t	add_token_parenthese(char *str, int32_t i, t_token *parent)
{
	add_tk(ft_strdup("("), TK_PARENTHESE_OPEN, i, parent);
	i = goto_closing_parenthese(str, i);
	if (str[i] != ')')
		parent->child->prev->is_continuation = true;
	else
		add_tk(ft_strdup(")"), TK_PARENTHESE_CLOSE, i, parent);
	i++;
	return (i);
}

/// @brief token group are tokens that may contains othet tokens or
/// may contains characters that may be indetified as a token.
/// we normally skip all tokens inside a group, the tokens inside
/// the group will be tokenize at next level
/// @param str
/// @param i
/// @param type
/// @param parent
/// @return
int32_t	add_token_group(char *str, int32_t i, t_token_type type,
		t_token *parent)
{
	if (type == TK_DOUBLEQUOTE)
		i = add_token_double_quote(str, i, parent);
	else if (type == TK_SINGLEQUOTE)
		i = add_token_single_quote(str, i, parent);
	else if (type == TK_COMMANDSUBSTITUTION_OPEN)
		i = add_token_substitution(str, i, parent, false);
	else if (type == TK_PARENTHESE_OPEN)
		i = add_token_parenthese(str, i, parent);
	else if (type == TK_ENVIRONEMENT_VAR)
		i = add_token_env(str, i, parent, false);
	return (i);
}
