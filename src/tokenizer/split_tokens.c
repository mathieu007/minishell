/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/06 07:19:02 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_token_environement(t_token *parent)
{
	char	*str;
	int32_t	start;
	int32_t	len;
	char	*env_value;
	t_token	*token;

	token = parent->child_tokens;
	str = parent->str;
	while (token && token->next)
	{	
		if (token->type == TK_DOUBLEQUOTE)
			build_token_environement(token);
		if (token->type == TK_ENVIRONEMENT_VAR)
			env_value = parse_env_var_value(token);
		else
			env_value = ft_strdup("");
		len = token->next->start - token->end;
		start = token->end;
		token->str = ft_strjoinfree2(env_value, ft_substr(str, start, len));
		token->str_len = ft_strlen(token->str);
		if (!token->inside_dbl_quotes)
			tokenize_space_tokens(token);
		token = token->next;
	}
}

/// @brief we do not tokenize single quote, nothing to do
/// 
/// @param parent 
void	split_token_groups(t_token *parent)
{
	char	*str;
	int32_t	start;
	int32_t	len;
	t_token	*token;

	token = parent->child_tokens;
	str = parent->str;
	while (token && token->next)
	{	
		len = token->next->start - token->end;
		start = token->end;
		token->str = ft_substr(str, start, len);
		token->str_len = len;
		if (token->type == TK_DOUBLEQUOTE)
			tokenize_dbl_quotes_tokens(token);
		else if (token->type == TK_PARENTHESE_OPEN)
			tokenize_cmd_sequence(token);
		else if (token->type == TK_COMMANDSUBSTITUTION_OPEN)
			tokenize_cmd_sequence(token);
		token = token->next;
	}
}

void	split_tokens(t_token *parent)
{
	char	*str;
	int32_t	start;
	int32_t	len;
	t_token	*token;

	token = parent->child_tokens;
	str = parent->str;
	while (token && token->next)
	{	
		len = token->next->start - token->end;
		start = token->end;
		token->str = ft_substr(str, start, len);
		token->str_len = len;
		token = token->next;
	}
}

t_cmd_seq	get_sequence_type(t_token *token)
{
	if (token && token->next)
	{
		if (token->next->type == TK_PIPE)
			return (CMD_PIPE);
		else if (token->next->type == TK_AND)
			return (CMD_LOG_AND);
		else if (token->next->type == TK_OR)
			return (CMD_LOG_OR);
		else if (token->next->type == TK_LESSLESS)
			return (CMD_FILEIN_APPPEND);
		else if (token->next->type == TK_LESS)
			return (CMD_FILEIN);
		else if (token->next->type == TK_GREATGREAT)
			return (CMD_FILEOUT_APPPEND);
		else if (token->next->type == TK_GREAT)
			return (CMD_FILEOUT);
		else if (token->next->type == TK_SEMICOLON)
			return (CMD_SEQUENTIAL);
	}
	if (token)
		return (CMD_SEQUENTIAL);
	else
		return (CMD_NONE);	
}

void	split_token_sequence(t_token *parent)
{
	char	*str;
	int32_t	start;
	int32_t	len;
	t_token	*token;

	token = parent->child_tokens;
	str = parent->str;
	while (token && token->next)
	{	
		len = token->next->start - token->end;
		start = token->end;
		token->str = ft_substr(str, start, len);
		token->str_len = len;
		token->cmd_seq_type = get_sequence_type(token);
		token->child_tokens = tokenize_group_tokens(token);
		token = token->next;
	}
}
