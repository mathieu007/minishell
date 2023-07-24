/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_environement.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 17:28:00 by math             ###   ########.fr       */
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
