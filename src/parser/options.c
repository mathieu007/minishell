/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/18 14:12:43 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	get_option_len(char *option)
{
	int32_t	len;

	len = 0;
	while (*option)
	{
		len++;
		option++;
	}
	return (len);
}

// static int32_t	concat_options(t_token *token)
// {
// 	char	*str;
// 	int32_t count;
	
// 	str = token->str;	
// 	count = 0;
// 	while (token && str && (token->type == TK_DOUBLEQUOTE
// 		|| token->type == TK_SINGLEQUOTE
// 		|| token->type == TK_CLOSINGDOUBLEQUOTE
// 		|| token->type == TK_CLOSINGDOUBLEQUOTE))
// 	{
// 		get_option_len(str);
// 	}	
		
// 	return (count);
// }

static int32_t	concat_options_count(t_token *token)
{
	char	*str;
	int32_t count;
	

	str = token->str;
	count = 0;
	while (token && str && (token->type == TK_DOUBLEQUOTE
		|| token->type == TK_SINGLEQUOTE
		|| token->type == TK_CLOSINGDOUBLEQUOTE
		|| token->type == TK_CLOSINGDOUBLEQUOTE))
	{
		count = get_option_len(str);
		token = token->next;
	}			
	return (count);
}

static int32_t	options_count(t_token_group *group)
{
	int32_t		count;
	t_token		*token;
	char		*str;
	bool		table[256];

	count = 0;
	token = group->first_token;
	ft_memset(table, 0, 256);
	token = token->next;
	if (token->type != TK_DASH && token->type != TK_DASHDASH)
		return (count);
	while (token)
	{
		if (token->type == TK_DASH)
		{
			str = token->str;
			while (*str && !table[(int32_t)(*str)])
			{
				table[(int32_t)(*str)] = true;
				count++;
			}
			count += concat_options_count(token);
		}
		else if (token->type == TK_DASHDASH)
		{
			
			count++;
		}			
		token = token->next;
	}
	return (count);
}

char	**parse_quoted_options(t_token *token)
{
	int32_t	i;

	token = token->next;
	while (token)
	{	
		i = 0;
		if (token->type == TK_SINGLEQUOTE)
		{
			
			while (token->str[i])
			{
				i++;
			}
		}		
		token = token->next;
	}
	return (NULL);
}

char	**get_malloc_opts(t_token_group *group)
{
	char	**options;
	int32_t	count;
	
	count = options_count(group);
	if (count == 0)
		return (NULL);
	options = malloc(sizeof(char *) * count);
	if (options == NULL)
		return (NULL);
	return (options);
}

char	**get_options(t_token_group *group)
{
	int32_t	i;
	int32_t	opt_i;
	char	**options;
	t_token	*tk;
	int32_t len;

	options = get_malloc_opts(group);
	tk = group->first_token;
	i = 0;
	opt_i = 0;
	while (tk)
	{
		if (tk->type == TK_DASH)
		{
			len = get_option_len(tk->str);
			while (i < len)
			{
				options[opt_i++] = ft_strncpy(&(tk->str[i]), 1);
				i++;
			}
		}
		else if (tk->type == TK_DASHDASH)
			options[opt_i++] = ft_strdupn(tk->str, get_option_len(tk->str));
		tk = tk->next;
	}
	return (options);
}