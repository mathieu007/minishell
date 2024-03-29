/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/26 09:58:53 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_env_words(char *str, char **split)
{
	int32_t	i;
	int32_t	start;

	if (*str)
		return (0);
	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == ' ')
		{
			*split++ = ft_strncpy(&str[start], i - start);
			while (str[i] == ' ')
				i++;
			start = i;
		}
		else
			i++;
	}
	return (split);
}

int32_t	count_env_words(char *str)
{
	int32_t	count;

	count = 1;
	if (*str)
		return (0);
	while (*str)
	{
		if (*str == ' ')
		{
			while (*str == ' ')
				str++;
			if (*str)
				count++;
		}
		else
			str++;
	}
	return (count);
}

int32_t	count_wildcards_matches(char *pattern)
{
	int32_t	count;
	char	**patterns;
	char	*start_with;

	start_with = get_start_with(pattern);
	patterns = get_patterns(pattern);
	count = count_matches(patterns, start_with, get_end_with(pattern));
	free_2d_char_array(patterns);
	return (free(start_with), count);
}

int32_t	calculate_args_len(t_token *token, int32_t args_len)
{
	if (is_token_delimiter(token->type) && token->next && token->next->next
		&& (token->next->next->type == TK_SPACE
			|| token->next->next->type == TK_END))
		args_len++;
	else if (token->next->type == TK_SPACE || token->next->type == TK_END)
	{
		if (ft_strcontains(token->str, "*"))
			args_len += count_wildcards_matches(token->str);
		else
			args_len++;
	}
	return (args_len);
}

int32_t	get_args_len(t_token *token)
{
	int32_t	args_len;

	args_len = 5;
	token = token->child;
	if (!token)
		return (0);
	while (token)
	{
		if ((token->type == TK_SPACE || token->type == TK_END)
			&& !ft_strcontains(token->str, "*"))
			args_len++;
		else if ((token->type == TK_SPACE || token->type == TK_END)
			&& ft_strcontains(token->str, "*"))
			args_len = calculate_args_len(token, args_len);
		else if ((token->type == TK_START || token->type == TK_CMD)
			&& ft_strcontains(token->str, "*"))
			args_len = calculate_args_len(token, args_len);
		else if (!is_token_delimiter(token->type) 
			&& ft_strcontains(token->str, "*"))
			args_len = calculate_args_len(token, args_len);
		token = token->next;
	}
	return (args_len);
}

// int32_t	get_args_len(t_token *token)
// {
// 	int32_t	args_len;

// 	token = token->child;
// 	args_len = 0;
// 	if (!token)
// 		return (0);
// 	while (token)
// 	{
// 		if (token->str && token->str[0] && token->next)
// 		{
// 			while (token && token->next)
// 			{
// 				if (token->next->type == TK_SPACE
// 					|| token->next->type == TK_END)
// 				{
// 					if (ft_strcontains(token->str, "*"))
// 						args_len += count_wildcards_matches(token->str);
// 					else
// 						args_len++;
// 					break ;
// 				}
// 				token = token->next;
// 			}
// 		}
// 		token = token->next;
// 	}
// 	return (args_len);
// }
