/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_environement.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/20 14:10:50 by mroy             ###   ########.fr       */
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
