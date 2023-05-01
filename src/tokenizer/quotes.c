/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/01 15:19:56 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	tokenize_double_quote(char *str, int32_t i)
{
	t_token_type	token_type;

	if (str == NULL)
		return (NULL);
	add_token(str, i, TK_DOUBLEQUOTE);
	i++;
	while (str[i] != NULL)
	{
		if (str[i] == '\\')
			i++;
		if (is_closing_double_quote(str, i))
		{
			add_token(&str[i], i, TK_DOUBLEQUOTE);
			return (i);
		}
		i++;
	}
	return (i);
}

int32_t	tokenize_single_quote(char *str, int32_t i)
{
	t_token_type	token_type;

	if (str == NULL)
		return (NULL);
	add_token(str, i, TK_SINGLEQUOTE);
	i++;
	while (str[i] != NULL)
	{
		if (is_closing_single_quote(str, i))
		{			
			add_token(&str[i], i, TK_SINGLEQUOTE);
			return (i);
		}
		if (is_escaped_single_quote(str, i))
			i++;
		i++;
	}
	return (i);
}

