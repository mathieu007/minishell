/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 08:05:05 by mroy              #+#    #+#             */
/*   Updated: 2023/05/09 14:07:06 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/// @brief simply get the end of a cmd sequence.
/// @param str 
/// @return 
char	*get_end_of_cmd(char *str)
{
	t_token_type	type;
	int32_t 		i;
	
	i = 0;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		if (type_is_end_of_seq(type))
			return (&str[i + get_token_type_len(type)]);
		i++;
	}
	return (&str[i]);
}