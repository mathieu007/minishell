/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/18 15:41:40 by mroy             ###   ########.fr       */
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
		len = closing_token->pos - token->pos - 2;
		arg = malloc(len + 1);
		arg = ft_substr(str, token->pos + 1, len);
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
		len = closing_token->pos - token->pos - 2;
		arg = malloc(len + 1);
		arg = ft_substr(str, token->pos + 1, len);
		return (arg);
	}
	return (NULL);
}

char **add_env_words(char *str, char **split)
{
	int32_t i;
	int32_t start;
	char	**start_ptr;

	if (*str)
		return (0);
	i = 0;
	start = 0;
	start_ptr = split;
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
			i++;;
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

int32_t	get_args_len(t_token_group *group)
{
	int32_t	args_len;
	t_token	*token;
	t_token	*prev_token;

	args_len = 0;
	token = group->first_token;
	if (!token)
		return (0);
	prev_token = token;
	while (token)
	{
		if (token->type == TK_SPACE)
			args_len++;
		else if (token->type == TK_ENVIRONEMENT_VAR)
			args_len += count_env_words(token->str);
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
		else if (token->type == TK_SPACE || token->type == TK_CMD_SEQ_END)
		{
			arg_start_pos = start_token->pos + start_token->token_len;
			arg_len = token->pos - start_token->pos;
			*arg = ft_substr(str, arg_start_pos, arg_len);
			token = token->next;
			return (token);
		}
		else
			token = token->next;
	}
	return (token);
}

void	get_args(t_token_group *group, char **split)
{
	int32_t	split_i;
	t_token	*token;
	char	*arg;
	char	*str;

	token = group->first_token;
	split_i = 0;
	str = group->str;
	arg = NULL;
	while (token)
	{
		arg = NULL;
		if (token->type == TK_SPACE)
			split[split_i++] = ft_strncpy(token->str, token->tolal_len);
		else if (token->type == TK_ENVIRONEMENT_VAR)
			split = add_env_words(token->str, split);
		token = token->next;
	}
}

char	**parse_args(t_token_group *group)
{
	int32_t	args_len;
	char	**split;

	args_len = get_args_len(group);
	split = malloc((args_len + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	split[args_len] = NULL;
	get_args(group, split);
	return (split);
}
