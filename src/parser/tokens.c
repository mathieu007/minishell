/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/04/30 08:25:59 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Single quote priority Number 1.
/// We delete any tokens that are inside single quote string,
/// single quote string are string literal, so any tokens inside 
/// a string literal are not valid, except the backslash single quote \'
/// @param tokens 
void	clean_single_quote(t_token *tokens)
{
	bool	is_open;

	is_open = false;
	while (tokens != NULL)
	{
		if (is_escaped_single_quote(tokens))
			tokens = tokens->next->next;
		if (tokens->type != TK_UNKNOWN && tokens->type
			!= TK_SINGLEQUOTE && is_open)
		{
			remove_token(tokens);
			decrement_counter(tokens->type);
		}			
		if (tokens->type == TK_SINGLEQUOTE && !is_open)
			is_open = true;
		else if (tokens->type == TK_SINGLEQUOTE && is_open)
			is_open = false;

		if (tokens->next != NULL)
			tokens = tokens->next;
	}
}


t_token	*clean_tokens(t_token *tokens)
{
	if (tokens == NULL)
		return (NULL);

	while (tokens != NULL)
	{
		
	}
}

void	tokenize(char *str)
{
	int32_t			i;
	t_token_type	token_type;
	int32_t			len;
	bool			in_str_literal;

	i = 0;
	len == ft_strlen(str);
	while (str[i] != '\0')
	{
		token_type = get_token_type(str);
		if (token_type == TK_UNKNOWN)
		{
			i++;
			continue ;
		}
		increment_counter(token_type);
		add_token(&str[i], i, token_type);
		i++;
	}
}
