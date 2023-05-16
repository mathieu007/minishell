/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 08:36:41 by mroy             ###   ########.fr       */
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
		if (!is_sibling_quote(token) || token->type == TK_SPACE
			|| ((token->type == TK_CMD_SEQ_END)
				&& prev_token->pos != token->pos))
		{
			prev_token = token;
			args_len++;
		}
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
	str = group->start;
	arg = NULL;
	while (token)
	{
		arg = NULL;
		if (token->type == TK_DOUBLEQUOTE || token->type == TK_SINGLEQUOTE)
			token = get_quotes_str(token, str, &arg);
		else if (token->type == TK_SPACE)
			token = get_space_str(token, str, &arg);
		if (arg)
			split[split_i++] = arg;
		token = token->next;
	}
}
