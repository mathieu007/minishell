/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/15 16:46:23 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int32_t	options_count(t_token_group *group)
{
	int32_t		count;
	t_token		*token;
	char		*str;
	bool		table[256];

	count = 0;
	token = group->first;
	ft_memset(table, 0, 256);
	while (token)
	{
		if (token->type == TK_DASH)
		{
			str = &(token->start)[1];
			while (*str && *str != ' ' && !table[(int32_t)(*str)])
			{
				table[(int32_t)(*str)] = true;
				count++;
			}
		}
		else if (token->type == TK_DASHDASH)
			count++;
		token = token->next;
	}
	return (count);
}

int32_t	get_option_len(char *option)
{
	int32_t	len;

	len = 0;
	while (*option != ' ' || *option != '\t')
	{
		len++;
		option++;
	}
	return (len);
}

char	**get_options(t_token_group *group)
{
	int32_t	count;
	int32_t	i;
	char	**options;
	t_token	*tk;

	count = options_count(group);
	if (count == 0)
		return (NULL);
	options = malloc(sizeof(char *) * count);
	if (options == NULL)
		return (NULL);
	tk = group->first;
	i = 0;
	while (tk)
	{
		if (tk->type == TK_DASH)
			options[i++] = ft_strdupn(tk->start, get_option_len(tk->start));
		else if (tk->type == TK_DASHDASH)
			options[i++] = ft_strdupn(tk->start, get_option_len(tk->start));
		tk = tk->next;
	}
	return (options);
}