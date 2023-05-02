/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/02 13:22:57 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int32_t	tokenize_parenthese(char *str, int32_t i)
// {
// 	t_token_type	token_type;

// 	if (str == NULL)
// 		return (NULL);
// 	add_token(str, i, TK_PARENTHESES_OPEN);
// 	i++;
// 	while (str[i] != NULL)
// 	{
// 		if (str[i] == '\\')
// 			i++;
// 		if (is_closing_parenthese(str, i))
// 		{
// 			add_token(str, i, TK_PARENTHESES_CLOSE);
// 			return (i);
// 		}	
// 		i++;
// 	}
// 	return (i);
// }
