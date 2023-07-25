/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 12:03:25 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_token_expansion_str(char *str)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	i = 0;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type);
		if (type == TK_SINGLEQUOTE)
			i = skip_token_single_quote(str, type, i);
		else if (type == TK_COMMANDSUBSTITUTION_OPEN
			|| type == TK_DOLLAR_SIGN_CURLYBRACE
			|| type == TK_ENVIRONEMENT_VAR || type == TK_LAST_CMD_EXIT)
			return (true);
		else
			i += t_len;
	}
	return (false);
}
