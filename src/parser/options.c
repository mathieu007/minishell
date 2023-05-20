/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/20 08:46:51 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int32_t	options_count(t_token_group *group, bool *table)
{
	int32_t		count;
	t_token		*token;
	char		*str;

	count = 0;
	token = group->first_token;
	token = token->next;
	while (token)
	{
		if (token->type == TK_DASH)
		{
			str = &(token->str[1]);
			while (*str && !table[(int32_t)(*str)])
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

char	**get_malloc_opts(t_token_group *group, bool *table)
{
	char	**options;
	int32_t	count;

	count = options_count(group, table);
	if (count == 0)
		return (NULL);
	options = malloc(sizeof(char *) * (count + 1));
	if (options == NULL)
		return (NULL);
	options[count] = NULL;
	return (options);
}

int32_t	add_single_dash_option(char	**options, bool *table)
{
	int32_t	i;
	int32_t	opt_i;
	char	c;

	i = 0;
	opt_i = 0;
	while (i < 256)
	{
		if (table[i])
		{
			c = (char)i;
			options[opt_i++] = ft_strjoin("-", &c);
			printf("option:%s\n", options[opt_i - 1]);
		}
		i++;
	}
	return (opt_i);
}

/// @brief TODO not WORKING
/// @param group 
/// @return 
char	**get_options(t_token_group *group)
{
	int32_t	opt_i;
	char	**options;
	t_token	*tk;
	bool	table[256];

	ft_memset(table, 0, 256);
	options = get_malloc_opts(group, &table[0]);
	opt_i = add_single_dash_option(options, &table[0]);
	tk = group->first_token;
	while (tk)
	{
		if (tk->type == TK_DASHDASH)
		{
			options[opt_i++] = ft_strdup(tk->str);
			printf("option:%s\n", options[opt_i - 1]);
		}
		tk = tk->next;
	}
	return (options);
}
