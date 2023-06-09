/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/05 14:39:42 by math             ###   ########.fr       */
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

char **add_env_words(char *str, char **split)
{
	int32_t i;
	int32_t start;

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
	int32_t count;

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

	token = token->child_tokens;
	args_len = 0;
	if (!token)
		return (0);
	while (token)
	{
		if (token->type == TK_SPACE)
			args_len++;
		else if (token->type == TK_END)
			args_len++;
		token = token->next;
	}
	return (args_len);
}

t_token	*get_space_str(t_token *token, char *str, char **arg)
{
	t_token	*start_token;
	int32_t	arg_len;
	int32_t	arg_start_pos;

	token = token->next;
	start_token = token;
	while (token)
	{
		if (token->type == TK_SINGLEQUOTE || token->type == TK_DOUBLEQUOTE)
			token = get_quotes_str(token, str, arg);
		else if (token->type == TK_SPACE || token->type == TK_END)
		{
			arg_start_pos = start_token->start + start_token->token_len;
			arg_len = token->start - start_token->start;
			*arg = ft_substr(str, arg_start_pos, arg_len);
			token = token->next;
			return (token);
		}
		else
			token = token->next;
		
	}
	return (token);
}

void	set_args(t_token *token, char **split)
{
	char	*cpy;
	int32_t	i;

	i = 0;
	token = token->child_tokens;
	while (token)
	{
		if (token->type == TK_SPACE)
			split[++i] = ft_strdup(token->str);
		else if (token->type == TK_DASH)
			split[i] = ft_strjoin(split[i], token->token_str);
		else if (token->type == TK_DASHDASH)
			split[i] = ft_strjoin(split[i], token->token_str);
		else
		{
			cpy = split[i];
			split[i] = ft_strjoin(split[i], token->str);
			free(cpy);
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
	split = malloc((args_len + 1) * sizeof(char *));
	while (i < args_len)
	{
		split[i] = NULL;
		i++;
	}
	if (!split)
		return (NULL);
	split[args_len] = NULL;
	set_args(token, split);
	return (split);
}
