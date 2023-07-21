/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*add_tk(char *token_str, t_token_type type, int32_t i, t_token *parent)
{
	t_token	*token;

	token = add_token(i, type, parent);
	token->token_len = ft_strlen(token_str);
	token->token_str = free_ptr(token->token_str);
	token->token_str = ft_strdup(token_str);
	token->end = i + token->token_len;
	return (token);
}

static bool	is_any_of(char c, char *values)
{
	while (*values)
	{
		if (*values == c)
			return (true);
		values++;
	}
	return (false);
}

static int32_t	check_newline_syntax_error(char *str, int32_t len, int32_t i)
{
	char	*syntax_error;

	syntax_error = "syntax error near unexpected token: newline";
	i += len;
	while (str[i] && str[i] == ' ')
		i++;
	if (!str[i])
	{
		get_process()->syntax_error = true;
		write_err(2, syntax_error);
		return (-1);
	}
	return (i);
}

int32_t	check_syntax_error_before_near(char *str, int32_t i, char *token_err)
{
	int32_t	start;
	char	*illegal_token;
	char	*c;

	start = i;
	illegal_token = "syntax error near unexpected token `";
	if (i > 0)
		i--;
	while (str[i] && str[i] == ' ')
		i--;
	if (is_any_of(str[i], token_err))
	{
		get_process()->syntax_error = true;
		c = ft_substr(str, start, 1);
		c = ft_strjoinfree(c, "'");
		write_err2(2, illegal_token, c);
		return (-1);
	}
	return (i);
}

int32_t	check_syntax_error_after_near(char *str, int32_t i, char *token_err)
{
	char		*illegal_token;
	t_process	*proc;
	char		*c;

	proc = get_process();
	illegal_token = "syntax error near unexpected token `";
	while (str[i] && str[i] == ' ')
		i++;
	if (is_any_of(str[i], token_err))
	{
		get_process()->syntax_error = true;
		c = ft_substr(str, i, 1);
		c = ft_strjoinfree(c, "'");
		write_err2(2, illegal_token, c);
		free(c);
		return (-1);
	}
	return (i);
}

static int32_t	check_redirection_syntax_errors(char *str, t_token_type type, 
	int32_t i)
{
	int32_t		len;
	t_process	*proc;

	proc = get_process();
	if (type == TK_LESSLESS)
		proc->has_here_doc = true;
	len = get_token_len(&str[i], type, false);
	i = check_newline_syntax_error(str, len, i);
	if (i != -1)
		i = check_syntax_error_after_near(str, i, "<>|&;()#");
	return (i);
}

static bool	has_syntax_errors(int32_t i, int32_t tk_len, t_token *parent)
{
	if (tk_len != 0 && check_syntax_error_before_near(parent->str, i,
			"|&;(<>") == -1)
		return (true);
	if (check_syntax_error_after_near(parent->str, i + tk_len, "|&;)<>") == -1)
		return (true);
	return (false);
}

bool	is_continuation(char *str)
{
	int32_t	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (!str[i])
		return (true);
	return (false);
}

int32_t	check_sequence_continuation(int32_t i, t_token_type	type, 
	t_token *parent)
{
	int32_t			len;
	char			*str;
	t_redirect		*redir;

	len = get_token_len(&parent->str[i], type, false);
	str = &parent->str[i + len];
	if (is_continuation(str))
	{
		redir = exec_continuation(parent);
		unlink_files_redirections(redir);
		free_t_redirect(redir);
		return (check_sequence_continuation(i, type, parent));
	}
	return (i + len);
}

int32_t	check_sequence_syntax_errors(int32_t i, t_token_type type,
		t_token *parent)
{
	int32_t	len;

	len = get_token_len(&parent->str[i], type, false);
	if (type == TK_SEMICOLON && has_syntax_errors(i++, len, parent))
		return (-1);
	else if (type == TK_OR && has_syntax_errors(i++, len, parent))
		return (-1);
	else if (type == TK_AND && has_syntax_errors(i++, len, parent))
		return (-1);
	else if (type == TK_PIPE && has_syntax_errors(i++, len, parent))
		return (-1);
	else
		check_sequence_continuation(i, type, parent);
	if (len == 2)
		return (i + 1);
	return (i);
}

void	check_continuations_and_error(t_token *token)
{
	int32_t			i;
	t_token_type	type;

	i = 0;
	while (token->str[i])
	{
		type = get_token_type(&token->str[i]);
		if (type == TK_DOUBLEQUOTE)
			i = check_dbl_quotes_continuation(i, token);
		else if (type == TK_SINGLEQUOTE)
			i = check_sgl_quotes_continuation(i, token);
		else if (type == TK_COMMANDSUBSTITUTION_OPEN)
			i = check_substitution_continuation(i, token);
		else if (type == TK_PARENTHESE_OPEN)
			i = check_parenthese_continuation(i, token);
		else if (type == TK_DOLLAR_SIGN_CURLYBRACE)
			i = check_environement_continuation(i, token);
		else if (is_token_redirection(type))
			i = check_redirection_syntax_errors(token->str, type, i);
		else if (is_sequence_type(type))
			i = check_sequence_syntax_errors(i, type, token);
		if (i == -1)
			return ;
		if (token->str[i])
			i++;
	}
}

/// @brief we add a root token that contains the full string
/// it's no very usefull, but i just need a root node.
/// @param str
/// @return
t_token	*tokenize(char *str)
{
	t_process	*proc;
	t_token		*root;

	proc = get_process();
	str = ft_strtrim(str, " ");
	root = add_tk("", TK_START, 0, NULL);
	root->str = str;
	proc->tokens = root;
	check_continuations_and_error(root);
	dispatch_tokenizer(root);
	return (proc->tokens);
}
