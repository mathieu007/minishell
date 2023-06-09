/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/05 14:39:16 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int32_t	options_count(t_token *token, bool *table)
{
	int32_t		count;
	char		*str;

	count = 0;
	token = token->child_tokens;
	token = token->next;
	while (token)
	{
		if (token->type == TK_DASH)
		{
			str = &(token->str[1]);
			while (*str)
			{
				if (!table[(int32_t)(*str)])
				{
					table[(int32_t)(*str)] = true;
					count++;
				}
				str++;
			}
		}
		else if (token->type == TK_DASHDASH)
			count++;
		token = token->next;
	}
	return (count);
}

char	**get_malloc_opts(t_token *token, bool *table)
{
	char	**options;
	int32_t	count;

	count = options_count(token, table);
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
	char	c[2];

	i = 0;
	opt_i = 0;
	c[1] = '\0';
	while (i < 256)
	{
		if (table[i])
		{
			c[0] = (char)i;
			options[opt_i++] = ft_strjoin("-", &c[0]);
		}
		i++;
	}
	return (opt_i);
}

/// @brief TODO not WORKING
/// @param group 
/// @return 
char	**get_options(t_token *token)
{
	int32_t	opt_i;
	char	**options;
	t_token	*tk;
	bool	table[256];

	ft_memset(table, 0, 256);
	options = get_malloc_opts(token, &table[0]);
	opt_i = add_single_dash_option(options, &table[0]);
	tk = token->child_tokens;
	while (tk)
	{
		if (tk->type == TK_DASHDASH)
			options[opt_i++] = ft_strdup(tk->str);
		tk = tk->next;
	}
	return (options);
}
