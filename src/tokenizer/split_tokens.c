
#include "minishell.h"

// char	*build_dbl_quote_token_env(t_token *token)
// {
// 	t_token	*child;
// 	char	*value;
// 	char	*str;

// 	child = token->child;
// 	str = NULL;
// 	value = NULL;
// 	while (child)
// 	{
// 		if (child->type == TK_ENVIRONEMENT_VAR)
// 			value = parse_env_var_value(child);
// 		else
// 			value = ft_strdup(child->str);
// 		str = ft_strjoinfree2(str, value);
// 		child = child->next;
// 	}
// 	return (str);
// }

// bool	is_not_expandable(t_token *child)
// {
// 	return (child->type != TK_ENVIRONEMENT_VAR
// 		&& child->type != TK_ENVIRONEMENT_VAR_CLOSE
// 		&& child->type != TK_COMMANDSUBSTITUTION_OPEN
// 		&& child->type != TK_COMMANDSUBSTITUTION_CLOSE
// 		&& child->type != TK_LAST_CMD_EXIT);
// }

// static t_token	*goto_closing_dbl_quote_token(t_token *token)
// {
// 	while (token && token->next)
// 	{
// 		if (token->type == TK_CLOSINGDOUBLEQUOTE)
// 			return (token);
// 		token = token->next;
// 	}
// 	return (token);
// }

// char	*parse_redirect_env_var_value(t_token *child, t_cmd_seq cmd_type)
// {
// 	char	*val;
// 	char	*ambigous_redirect;

// 	val = parse_env_var_value(child);
// 	if (child->type == TK_ENVIRONEMENT_VAR && child->prev
// 		&& child->prev->type != TK_SPACE && cmd_type == CMD_HEREDOC)
// 	{
// 		val = ft_strjoin(child->token_str, child->str);
// 		if (child->token_len == 2)
// 			val = ft_strjoinfree(val, "}");
// 	}
// 	else if (child->prev && child->prev->type != TK_SPACE && val
// 			&& val[0] == ' ')
// 	{
// 		ambigous_redirect = ft_strjoin(child->prev->str, child->token_str);
// 		ambigous_redirect = ft_strjoinfree(ambigous_redirect, child->str);
// 		if (child->token_len == 2)
// 			ambigous_redirect = ft_strjoinfree(ambigous_redirect, "}");
// 		get_process()->errnum = 1;
// 		write_err2(1, ambigous_redirect, ": ambiguous redirect\n");
// 		return (free(val), NULL);
// 	}
// 	return (val);
// }

// void	*build_redir_token(t_token *token, t_cmd_seq cmd_type)
// {
// 	char	*val;
// 	t_token	*child;
// 	char	*rebuild_str;

// 	child = token->child;
// 	rebuild_str = NULL;
// 	while (child && child->next)
// 	{
// 		if (is_not_expandable(child))
// 			rebuild_str = ft_strjoinfree(rebuild_str, child->token_str);
// 		if (child->type == TK_DOUBLEQUOTE)
// 		{
// 			val = build_dbl_quote_token_env(child);
// 			rebuild_str = ft_strjoinfree(rebuild_str, val);
// 			child = goto_closing_dbl_quote_token(child);
// 			rebuild_str = ft_strjoinfree(rebuild_str, child->token_str);
// 			val = ft_strdup(child->str);
// 		}
// 		else if (child->type == TK_ENVIRONEMENT_VAR)
// 		{
// 			val = parse_redirect_env_var_value(child, cmd_type);
// 			if (!val)
// 				return (free(rebuild_str), NULL);
// 		}
// 		else
// 			val = ft_strdup(child->str);
// 		rebuild_str = ft_strjoinfree2(rebuild_str, val);
// 		child = child->next;
// 	}
// 	token->child = free_t_tokens(token->child);
// 	token->str = free_ptr(token->str);
// 	token->str = ft_strtrim(rebuild_str, " ");
// 	token->last = NULL;
// 	token->token_str = free_ptr(token->token_str);
// 	if (rebuild_str)
// 		free(rebuild_str);
// 	tokenize_group_tokens(token);
// 	return (token);
// }

void	*build_token_environement(t_token *token)
{
	char		*val;
	char		*str;
	t_process	*proc;
	t_token		*child;

	proc = get_process();
	str = NULL;
	if (token->type == TK_PARENTHESE_OPEN)
		return (token);
	if (!has_token_expansion(token))
		return (space_quotes_tokenizer(token));
	child = token->child;
	while (child && child->next)
	{
		if (child->type == TK_COMMANDSUBSTITUTION_OPEN)
			val = ft_strdup(child->str);
		else if (child->type == TK_ENVIRONEMENT_VAR)
			val = parse_env_var_value(child);
		else if (child->type == TK_LAST_CMD_EXIT)
			val = ft_itoa(proc->last_errnum);
		else
			val = ft_strdup(child->str);
		str = ft_strjoinfree2(str, val);
		child = child->next;
	}
	token->child = free_t_tokens(token->child);
	token->str = free_ptr(token->str);
	token->str = str;
	token->last = NULL;
	// token->token_str = free_ptr(child->token_str);
	return (space_quotes_tokenizer(token));
}

// void	build_token_environement(t_token *token)
// {
// 	char		*val;
// 	t_token		*child;
// 	char		*str;
// 	t_process	*proc;

// 	proc = get_process();
// 	child = token->child;
// 	str = NULL;
// 	while (child && child->next)
// 	{
// 		if (is_not_expandable(child))
// 			str = ft_strjoinfree2(str, ft_strdup(child->token_str));
// 		if (child->type == TK_DOUBLEQUOTE)
// 		{
// 			val = build_dbl_quote_token_env(child);
// 			str = ft_strjoinfree2(str, val);
// 			child = goto_closing_dbl_quote_token(child);
// 			str = ft_strjoinfree2(str, ft_strdup(child->token_str));
// 			val = ft_strdup(child->str);
// 		}
// 		else if (child->type == TK_ENVIRONEMENT_VAR)
// 			val = parse_env_var_value(child);
// 		else if (child->type == TK_LAST_CMD_EXIT)
// 			val = ft_itoa(proc->last_errnum);
// 		else
// 			val = ft_strdup(child->str);
// 		str = ft_strjoinfree2(str, val);
// 		child = child->next;
// 	}
// 	token->child = free_t_tokens(token->child);
// 	token->str = free_ptr(token->str);
// 	token->str = ft_strtrim(str, " ");
// 	token->last = NULL;
// 	token->token_str = free_ptr(token->token_str);
// 	if (str)
// 		free(str);
// 	tokenize_group_tokens(token);
// }

// void	split_token_groups(t_token *parent)
// {
// 	char	*str;
// 	int32_t	start;
// 	int32_t	len;
// 	t_token	*token;

// 	token = parent->child;
// 	str = parent->str;
// 	while (token && token->next)
// 	{
// 		len = token->next->start - token->end;
// 		start = token->start + token->token_len;
// 		token->str = free_ptr(token->str);
// 		token->str = ft_substr(str, start, len);
// 		if (token->type == TK_DOUBLEQUOTE)
// 			tokenize_dbl_quotes_tokens(token);
// 		else if (token->type == TK_PARENTHESE_OPEN)
// 			tokenize_cmd_sequence(token);
// 		else if (token->type == TK_COMMANDSUBSTITUTION_OPEN)
// 			tokenize_cmd_sequence(token);
// 		token = token->next;
// 	}
// }

// void	split_tokens(t_token *parent)
// {
// 	char	*str;
// 	int32_t	start;
// 	int32_t	len;
// 	t_token	*token;

// 	token = parent->child;
// 	str = parent->str;
// 	while (token && token->next)
// 	{
// 		len = token->next->start - token->end;
// 		start = token->end;
// 		token->str = free_ptr(token->str);
// 		token->str = ft_substr(str, start, len);
// 		token = token->next;
// 	}
// }

// void	split_token_cmd(t_token *parent)
// {
// 	char	*str;
// 	int32_t	start;
// 	int32_t	len;
// 	t_token	*token;
// 	char	*tmp_str;

// 	token = parent->child;
// 	str = parent->str;
// 	while (token && token->next)
// 	{
// 		len = token->next->start - token->end;
// 		start = token->start + token->token_len;
// 		token->str = free_ptr(token->str);
// 		tmp_str = ft_substr(str, start, len);
// 		token->str = ft_strtrimfree(tmp_str, " ");
// 		if (token->type == TK_PARENTHESE_OPEN)
// 			token->child = NULL;
// 		else if (token->str[0])
// 			token->child = tokenize_group_tokens(token);
// 		token = token->next;
// 	}
// }

// /// @brief once we have identified all command tokens we separate them
// /// it's just an ft_substr from token to next token
// /// @param parent
// void	split_token_redirection(t_token *parent)
// {
// 	char	*str;
// 	int32_t	start;
// 	int32_t	len;
// 	t_token	*token;
// 	char	*temp;

// 	token = parent->child;
// 	str = parent->str;
// 	while (token && token->next)
// 	{
// 		len = token->next->start - token->end;
// 		start = token->start + token->token_len;
// 		temp = ft_substr(str, start, len);
// 		token->str = free_ptr(token->str);
// 		token->str = ft_strtrimfree(temp, " ");
// 		free(temp);
// 		token->child = tokenize_group_tokens(token);
// 		token = token->next;
// 	}
// }

// void	split_token_semicolon(t_token *parent)
// {
// 	char	*str;
// 	int32_t	start;
// 	int32_t	len;
// 	t_token	*token;

// 	token = parent->child;
// 	str = parent->str;
// 	while (token && token->next)
// 	{
// 		len = token->next->start - token->end;
// 		start = token->start + token->token_len;
// 		token->str = free_ptr(token->str);
// 		token->str = ft_strtrimfree(ft_substr(str, start, len), " ");
// 		token->child = tokenize_cmd_sequence(token);
// 		token = token->next;
// 	}
// }
