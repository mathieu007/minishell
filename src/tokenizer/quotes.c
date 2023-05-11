/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 15:20:49 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	tokenize_double_quote(char *str, int32_t i, t_token_group *group)
{
	if (str == NULL)
		return (0);
	add_token(str, i, TK_DOUBLEQUOTE, group);
	i++;
	while (str[i])
	{
		if (str[i] == '\\')
			i++;
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
	if (str == NULL)
		return (0);
	add_token(str, i, TK_SINGLEQUOTE, group);
	i++;
	while (str[i])
	{
		if (is_closing_single_quote(str, i))
		{			
			add_token(&str[i], i, TK_SINGLEQUOTE, group);
			return (i);
		}
		i++;
	}
	return (i);
}
