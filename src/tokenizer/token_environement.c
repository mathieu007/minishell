/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_environement.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/22 15:49:13 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	get_token_env_len(char *str)
{
	int32_t	pos;

	pos = 1;
	if (str[pos] == '{')
		pos++;
	while (str[pos] && ft_isalnum(str[pos]) == 1)
		pos++;
	if (str[pos] == '}')
		pos++;
	return (pos);
}

int32_t	add_token_cmd_substitution(char *str, int32_t pos, t_token *parent,
	bool inside_dbl_quotes)
{
	t_token		*token;

		token = add_tk("${", TK_ENVIRONEMENT_VAR, pos++, parent);
		token->inside_dbl_quotes = inside_dbl_quotes;
		while (str[pos] && str[pos] != '}')
			pos++;
		if (str[pos] != '}')
			token->is_continuation = true;
		else
			add_tk(")", TK_ENVIRONEMENT_VAR_CLOSE, pos, parent);
	return (token->end);
}

int32_t	add_token_env(char *str, int32_t pos, t_token *parent,
	bool inside_dbl_quotes)
{
	t_token		*token;

	pos++;
	if (str[pos] == '{')
	{
		token = add_tk("${", TK_ENVIRONEMENT_VAR, pos - 1, parent);
		pos++;
		while (str[pos] && str[pos] != '}')
			pos++;
		if (str[pos] != '}')
			token->is_continuation = true;
		else
			add_tk("}", TK_ENVIRONEMENT_VAR_CLOSE, pos, parent);
	}
	else
	{
		token = add_tk("$", TK_ENVIRONEMENT_VAR, pos - 1, parent);
		if (ft_isdigit(str[pos]) == 1)
			pos++;
		else
		{
			while (ft_isalnum(str[pos]) == 1 || str[pos] == '_')
				pos++;
		}
		add_tk("", TK_ENVIRONEMENT_VAR_CLOSE, pos, parent);
	}
	token->inside_dbl_quotes = inside_dbl_quotes;
	return (token->end);
}

// /// @brief separators are spaces, this function does not execute inside
// /// token string.
// /// @param parent 
// /// @return 
// t_token	*tokenize_expandable_tokens(t_token *parent)
// {
// 	int32_t			i;
// 	t_token_type	type;
// 	int32_t			t_len;
// 	char			*str;
// 	bool			in_dbl_quotes;

// 	i = 0;
// 	in_dbl_quotes = parent->type == TK_DOUBLEQUOTE;
// 	str = ft_strtrim(parent->str, " ");
// 	add_tk(ft_strdup(""), TK_START, 0, parent);
// 	while (str[i])
// 	{
// 		type = get_token_type(&str[i]);
// 		t_len = get_token_len(&str[i], type, false);
// 		if (type == TK_COMMANDSUBSTITUTION_OPEN)
// 			i = add_token_cmd_substitution(str, i, parent, in_dbl_quotes);
// 		if (type == TK_ENVIRONEMENT_VAR)
// 			i = add_token_env(str, i, parent, in_dbl_quotes);
// 		else
// 			i += t_len;
// 	}
// 	add_tk(ft_strdup(""), TK_END, i, parent);
// 	build_token_environement(parent);
// 	return (parent->child_tokens);
// }
