/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/04 06:43:05 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize(char *str)
{
	int32_t			i;
	t_token_type	token_type;
	int32_t			len;

	i = 0;
	len = ft_strlen(str);
	while (str[i] < len)
	{
		token_type = get_token_type(&str[i]);
		if (is_opening_single_quote(str, i))
			i += tokenize_single_quote(str, i);
		else if (is_opening_double_quote(str, i))
			i += tokenize_double_quote(str, i);
		else if (token_type != TK_UNKNOWN)
			add_token(str, i, token_type);
		i++;
	}
}
