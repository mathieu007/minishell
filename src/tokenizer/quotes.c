/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/17 11:19:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	tokenize_double_quote(char *str, int32_t i, t_token_group *group)
{
	t_token_type	type;
	int32_t			t_len;

	if (str == NULL)
		return (i);
	add_token(i , TK_DOUBLEQUOTE, group);
	i++;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_type_len(type);
		if (t_len == 0)
			t_len = 1;
		if (type == TK_DOUBLEQUOTE)
		{
			add_token(i, TK_CLOSINGDOUBLEQUOTE, group)->token_len = 1;
			return (i + 1);
		}
		else if (str_is_env_variable(&str[i]))
		{
			add_token(i, TK_ENVIRONEMENT_VAR, group)->tolal_len = get_env_var_name_len(&str[i]);
			i++;
		}
		else if (type != TK_UNKNOWN && type != TK_SPACE
			&& type != TK_SINGLEQUOTE)
			add_token(i, type, group);
		i += t_len;
	}
	return (i);
}

int32_t	tokenize_single_quote(char *str, int32_t i, t_token_group *group)
{
	t_token_type	type;
	int32_t			t_len;

	if (str == NULL)
		return (i);
	add_token(i, TK_SINGLEQUOTE, group);
	i++;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_type_len(type);
		if (t_len == 0)
			t_len = 1;
		if (type == TK_SINGLEQUOTE)
		{
			add_token(i, TK_CLOSINGSINGLEQUOTE, group)->token_len = 1;
			return (i + 1);
		}
		else
			i += t_len;
	}
	return (i);
}

// int32_t	tokenize_double_quote(char *str, int32_t i, t_token_group *group)
// {
// 	t_token_type	type;
// 	int32_t			t_len;
// 	t_token			*token;
// 	int32_t			start;

// 	if (str == NULL)
// 		return (i);
// 	i++;
// 	start = i;
// 	token = add_token(str, i, TK_DOUBLEQUOTE, group);
// 	while (str[i])
// 	{
// 		type = get_token_type(&str[i]);
// 		t_len = get_token_type_len(type);
// 		if (type == TK_DOUBLEQUOTE)
// 			break ;
// 		else if (type != TK_UNKNOWN)
// 			i += t_len;
// 		else
// 			i++;
// 	}
// 	i--;
// 	token->start = ft_strncpy(&str[start], i - start);
// 	return (i);
// }

// int32_t	tokenize_single_quote(char *str, int32_t i, t_token_group *group)
// {
// 	t_token_type	type;
// 	int32_t			t_len;
// 	t_token			*token;
// 	int32_t			start;

// 	if (str == NULL)
// 		return (i);
// 	i++;
// 	start = i;
// 	token = add_token(str, i, TK_SINGLEQUOTE, group);
// 	while (str[i])
// 	{
// 		type = get_token_type(&str[i]);
// 		t_len = get_token_type_len(type);
// 		if (type == TK_SINGLEQUOTE)
// 			break ;
// 		else if (type != TK_UNKNOWN)
// 			i += t_len;
// 		else
// 			i++;
// 	}
// 	i--;
// 	token->start = ft_strncpy(&str[start], i - start);
// 	return (i);
// }
