/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 21:26:21 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	tokenize_double_quote(char *str, int32_t i, t_token_group *group)
{
	t_token_type	token_type;

	if (str == NULL)
		return (NULL);
	add_token(str, i, TK_DOUBLEQUOTE, group);
	i++;
	while (str[i] != NULL)
	{
		// if (is_escaped_env_variable(str, i))
		// 	i++;
		if (str[i] == '\\')
			i++;
		// else if (is_env_variable(str, i))
		// 	i++;
		else if (is_closing_double_quote(str, i))
		{
			add_token(&str[i], i, TK_DOUBLEQUOTE, group);
			return (i);
		}
		else
			add_token(&str[i], i, get_token_type(&str[i]), group);
		i++;
	}
	return (i);
}

int32_t	tokenize_single_quote(char *str, int32_t i, t_token_group *group)
{
	t_token_type	token_type;

	if (str == NULL)
		return (NULL);
	add_token(str, i, TK_SINGLEQUOTE, group);
	i++;
	while (str[i] != NULL)
	{
		if (is_closing_single_quote(str, i))
		{			
			add_token(&str[i], i, TK_SINGLEQUOTE, group);
			return (i);
		}
		// if (is_escaped_single_quote(str, i, group))
		// 	i++;
		i++;
	}
	return (i);
}

