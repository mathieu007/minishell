/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 12:59:34 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_group	*tokenize_groups(char *str)
{
	int32_t			i;
	t_token_type	type;
	char			*start;

	i = 0;
	start = str;
	while (str[i])
	{
		type = get_token_type(&str[i]);
		if (type_is_end_of_seq(type))
		{
			if (get_token_type_len(type) == 2)
				i++;
			add_token_group(start, &str[i + 1]);	
			start = &str[i + 1];
		}
		i++;
	}
	if (start != &str[i])
		add_token_group(start, &str[i]);
	return (get_first_token_group());
}

t_token	*tokenize(char *str)
{
	int32_t			i;
	t_token_type	type;
	t_token_group	*group;
	t_token			*token;

	group = tokenize_groups(str);
	while (group)
	{
		i = 0;
		str = group->start;
		token = NULL;
		while (str[i])
		{			
			type = get_token_type(&str[i]);
			if (is_opening_single_quote(str, i))
				i += tokenize_single_quote(str, i, group);
			else if (is_opening_double_quote(str, i))
				i += tokenize_double_quote(str, i, group);
			else if (type != TK_UNKNOWN)
				token = add_token(str, i, type, group);
			if (get_token_type_len(type) == 2)
				i++;
			i++;
		}		
	}
	return (get_first_token());
}
