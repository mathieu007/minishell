/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/13 16:24:00 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*build_dbl_quote_token_env(t_token *token)
{
	t_token	*child;
	char	*value;
	char	*str;
	t_token	*temp;

	child = token->child_tokens;
	str = ft_strdup("");
	while (child)
	{
		if (child->type == TK_ENVIRONEMENT_VAR)
			value = parse_env_var_value(child);
		else
			value = ft_strdup(child->str);
		str = ft_strjoinfree2(str, value);
		temp = child;
		child = child->next;
		free_t_token(temp);
	}
	return (str);
}

bool	is_not_expandable(t_token *child)
{
	return (child->type != TK_ENVIRONEMENT_VAR
		&& child->type != TK_ENVIRONEMENT_VAR_CLOSE
		&& child->type != TK_COMMANDSUBSTITUTION_OPEN
		&& child->type != TK_COMMANDSUBSTITUTION_CLOSE);
}

static t_token	*goto_closing_dbl_quote_token(t_token *token)
{
	while (token && token->next)
	{
		if (token->type == TK_CLOSINGDOUBLEQUOTE)
			return (token);
		token = token->next;
	}
	return (token);
}

char	*parse_redirect_env_var_value(t_token *child, t_cmd_seq cmd_type)
{
	char	*val;
	char	*ambigous_redirect;

	val = parse_env_var_value(child);
	if (child->type == TK_ENVIRONEMENT_VAR
		&& child->prev && child->prev->type != TK_SPACE
		&& cmd_type == CMD_HEREDOC)
	{
		val = ft_strjoin(child->token_str, child->str);
		if (child->token_len == 2)
			val = ft_strjoinfree(val, "}");
	}
	else if (child->prev && child->prev->type != TK_SPACE
		&& val && val[0] == ' ')
	{
		ambigous_redirect = ft_strjoin(child->prev->str, child->token_str);
		ambigous_redirect = ft_strjoinfree(ambigous_redirect, child->str);
		if (child->token_len == 2)
			ambigous_redirect = ft_strjoinfree(ambigous_redirect, "}");
		get_process()->errnum = 1;
		write_err2(1, ambigous_redirect, ": ambiguous redirect");
		return (free(val), NULL);
	}
	return (val);
}

void	*build_redir_token_environement(t_token *token, t_cmd_seq cmd_type)
{
	char	*val;
	t_token	*child;
	char	*str;

	child = token->child_tokens;
	str = ft_strdup("");
	while (child && child->next)
	{	
		if (is_not_expandable(child))
			str = ft_strjoinfree(str, child->token_str);
		if (child->type == TK_DOUBLEQUOTE)
		{
			val = build_dbl_quote_token_env(child);
			str = ft_strjoinfree(str, val);
			child = goto_closing_dbl_quote_token(child);
			str = ft_strjoinfree(str, child->token_str);
			val = ft_strdup(child->str);
		}
		else if (child->type == TK_ENVIRONEMENT_VAR)
		{
			val = parse_redirect_env_var_value(child, cmd_type);
			if (!val)
				return (free(val), free(str), NULL);
		}
		else
			val = ft_strdup(child->str);
		str = ft_strjoinfree2(str, val);
		child = child->next;
	}
	free_t_tokens(token->child_tokens);
	token->child_tokens = NULL;
	free(token->str);
	token->str = ft_strtrim(str, " ");
	free(str);
	tokenize_group_tokens(token);
	return (token);
}

void	build_token_environement(t_token *token)
{
	char	*val;
	t_token	*child;
	char	*str;

	child = token->child_tokens;
	str = ft_strdup("");
	while (child && child->next)
	{	
		if (is_not_expandable(child))
			str = ft_strjoinfree2(str, ft_strdup(child->token_str));
		if (child->type == TK_DOUBLEQUOTE)
		{
			val = build_dbl_quote_token_env(child);
			str = ft_strjoinfree2(str, val);
			child = goto_closing_dbl_quote_token(child);
			str = ft_strjoinfree2(str, ft_strdup(child->token_str));
			val = ft_strdup(child->str);
		}
		else if (child->type == TK_ENVIRONEMENT_VAR)
			val = parse_env_var_value(child);
		else
			val = ft_strdup(child->str);
		str = ft_strjoinfree2(str, val);
		child = child->next;
	}
	free_t_tokens(token->child_tokens);
	token->child_tokens = NULL;
	free(token->str);
	token->str = ft_strtrim(str, " ");
	free(str);
	tokenize_group_tokens(token);
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
		start = token->start + token->token_len;
		token->str = ft_substr(str, start, len);
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
		token = token->next;
	}
}

t_cmd_seq	get_sequence_type(t_token *token)
{
	if (token && token->next)
	{
		if (token->type == TK_START
			&& (token->next->type != TK_PIPE
				&& token->next->type != TK_OR
				&& token->next->type != TK_AND))
			return (CMD_SEQUENTIAL);
		if (token->type == TK_PIPE || (token->next && token->next->type == TK_PIPE 
			&& token->type != TK_LESSLESS && token->type != TK_LESS
			&& token->type != TK_GREAT && token->type != TK_GREATGREAT))
			return (CMD_PIPE);
		else if (token->type == TK_OR || (token->type == TK_START && token->next->type == TK_OR))
			return (CMD_LOG_OR);
		else if (token->type == TK_AND || (token->type == TK_START && token->next->type == TK_AND))
			return (CMD_LOG_AND);
		else if (token->type == TK_LESSLESS)
			return (CMD_HEREDOC);
		else if (token->type == TK_LESS)
			return (CMD_FILEIN);
		else if (token->type == TK_GREATGREAT)
			return (CMD_FILEOUT_APPPEND);
		else if (token->type == TK_GREAT)
			return (CMD_FILEOUT);
		else if (token->type == TK_SEMICOLON)
			return (CMD_SEQUENTIAL);
		else if (token->type == TK_PARENTHESE_OPEN)
			return (CMD_GROUPING);
		else if (token->type == TK_COMMANDSUBSTITUTION_OPEN)
			return (CMD_SUBSTITUTION);
		else if (token->type == TK_START)
			return (CMD_SEQUENTIAL);
	}
	return (CMD_NONE);
}

t_token *set_cmd_sequence(t_token *token)
{
	t_token	*redir_token;
	
	token->cmd_seq_type = get_sequence_type(token);
	if (!is_redirection(token->cmd_seq_type) && token->next && is_redirection(get_sequence_type(token->next)))
	{
		redir_token = token->next;
		while (redir_token)
		{
			if (!is_redirection(get_sequence_type(redir_token)) && redir_token->type != TK_END)
				token->cmd_seq_type = get_sequence_type(redir_token);
			redir_token = redir_token->next;
		}
	}
	return (token);
}

/// @brief once we have identified all command tokens we separate them
/// it's just an ft_substr from token to next token
/// @param parent 
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
		start = token->start + token->token_len;
		token->str = ft_strtrim(ft_substr(str, start, len), " ");
		token = set_cmd_sequence(token);
		token->child_tokens = tokenize_group_tokens(token);
		token = token->next;
	}
}
