/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/20 13:56:01 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief get the single quote string without the quotes
/// @param token
/// @param str
/// @return
char	*get_single_quote_str(t_token *token, char *str)
{
	t_token	*closing_token;
	char	*arg;
	int32_t	len;

	closing_token = advance_to(token, TK_CLOSINGSINGLEQUOTE);
	if (closing_token != token)
	{
		len = closing_token->start - token->start - 2;
		arg = malloc(len + 1);
		arg = ft_substr(str, token->start + 1, len);
		return (arg);
	}
	return (NULL);
}

/// @brief get the double quote string without the quotes
/// @param token
/// @param str
/// @return
char	*get_double_quote_str(t_token *token, char *str)
{
	t_token	*closing_token;
	char	*arg;
	int32_t	len;

	closing_token = advance_to(token, TK_CLOSINGDOUBLEQUOTE);
	if (closing_token != token)
	{
		len = closing_token->start - token->start - 2;
		arg = malloc(len + 1);
		arg = ft_substr(str, token->start + 1, len);
		return (arg);
	}
	return (NULL);
}

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

int32_t	get_args_len(t_token *token)
{
	int32_t	args_len;

	token = token->child;
	args_len = 0;
	if (!token)
		return (0);
	while (token)
	{
		if (token->str && token->str[0] && token->next)
		{
			while (token && token->next)
			{
				if (token->next->type == TK_SPACE
					|| token->next->type == TK_END)
				{
					args_len++;
					break ;
				}
				token = token->next;
			}
		}
		token = token->next;
	}
	return (args_len);
}

void	set_args(t_token *token, char **split)
{
	int32_t	i;

	i = 0;
	token = token->child;
	while (token)
	{
		if (token->str && token->str[0] && token->next)
		{
			while (token && token->next)
			{
				split[i] = ft_strjoinfree(split[i], token->str);
				if (token->next->type == TK_SPACE
					|| token->next->type == TK_END)
				{
					i++;
					break ;
				}
				token = token->next;
			}
		}
		token = token->next;
	}
}

char	**parse_args(t_token *token)
{
	int32_t	args_len;
	char	**split;
	int32_t	i;

	i = 0;
	args_len = get_args_len(token);
	if (args_len == 0)
		return (NULL);
	split = malloc((args_len + 1) * sizeof(char *));
	if (!split)
		free_all_and_exit2(errno, "malloc error");
	while (i <= args_len)
	{
		split[i] = NULL;
		i++;
	}
	split[args_len] = NULL;
	set_args(token, split);
	return (split);
}
