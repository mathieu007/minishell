#include "minishell.h"

void	split_token_sequence(t_token *parent)
{
	char	*str;
	int32_t	start;
	int32_t	len;
	t_token	*token;

	token = parent->child;
	str = parent->str;
	while (token && token->next)
	{
		len = token->next->start - token->end;
		start = token->start + token->token_len;
		token->str = free_ptr(token->str);
		token->str = ft_strtrimfree(ft_substr(str, start, len), " ");
		token->child = dispatch_tokenizer(token);
		token = token->next;
	}
}

bool	is_any_of(char c, char *values)
{
	while (*values)
	{
		if (*values == c)
			return (true);
		values++;
	}
	return (false);
}

bool	has_syntax_error_before_near(char *str, int32_t i, char *token_err)
{
	int32_t	start;
	char	*illegal_token;

	start = i;
	illegal_token = "syntax error near unexpected token: ";
	if (i > 0)
		i--;
	while (str[i] && str[i] == ' ')
		i--;
	if (is_any_of(str[i], token_err))
	{
		get_process()->syntax_error = true;
		write_err2(2, illegal_token, &str[start]);
		return (true);
	}
	return (false);
}

bool	has_syntax_error_after_near(char *str, int32_t i, char *token_err)
{
	char	*illegal_token;

	illegal_token = "syntax error near unexpected token: ";
	if (str[i])
		i++;
	while (str[i] && str[i] == ' ')
		i++;
	if (is_any_of(str[i], token_err))
	{
		get_process()->syntax_error = true;
		write_err2(2, illegal_token, &str[i]);
		return (true);
	}
	return (false);
}

int32_t	add_sequence_token(int32_t i, char *tk_str, t_token_type type,
		t_token *parent)
{
	int32_t	len;

	// char		*str;
	// t_redirect	*redir;
	len = ft_strlen(tk_str);
	// str = &parent->str[i + len];
	// if (len != 0 && has_syntax_error_before_near(parent->str, i,
	// 		"|&;(<>"))
	// 	return (i + len);
	// if (has_syntax_error_after_near(parent->str, i + len, "|&;)<>"))
	// 	return (i + len);
	// if (is_continuation(str))
	// {
	// 	redir = exec_continuation(parent);
	// 	unlink_files_redirections(redir);
	// 	free_t_redirect(redir);
	// 	return (add_sequence_token(i, tk_str, type, parent));
	// }
	add_tk(tk_str, type, i, parent);
	i += len;
	return (i);
}

int32_t	add_semicolon_sequence_token(int32_t i, t_token_type type,
		t_token *parent)
{
	add_tk(";", type, i, parent);
	i++;
	return (i);
}

t_token	*sequence_semicolon_tokenizer(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	t_token_type	tk_type;

	i = 0;
	tk_type = get_token_type(";");
	if (!has_token_semicolon_sequence(parent))
		return (NULL);
	add_sequence_token(i, "", TK_START, parent);
	while (parent->str[i])
	{
		if (has_error())
			return (parent->child);
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (tk_type != type && is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == tk_type)
			i = add_semicolon_sequence_token(i, type, parent);
		else
			i += t_len;
	}
	add_tk("", TK_END, i, parent);
	split_token_sequence(parent);
	return (parent->child);
}

t_token	*sequence_tokenizer(char *tk, t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	t_token_type	tk_type;

	i = 0;
	tk_type = get_token_type(tk);
	if (!has_token_sequence(parent))
		return (NULL);
	add_sequence_token(i, "", tk_type, parent);
	while (parent->str[i])
	{
		if (has_error())
			return (parent->child);
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (tk_type != type && is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == tk_type)
			i = add_sequence_token(i, tk, type, parent);
		else
			i += t_len;
	}
	add_tk("", TK_END, i, parent);
	split_token_sequence(parent);
	return (parent->child);
}

t_token	*sequences_tokenizer(t_token *parent)
{
	t_token	*child;

	child = NULL;
	if (has_error())
		return (parent->child);
	if (has_token(";", parent))
		child = sequence_semicolon_tokenizer(parent);
	else if (has_token("||", parent))
		child = sequence_tokenizer("||", parent);
	else if (has_token("&&", parent))
		child = sequence_tokenizer("&&", parent);
	else if (has_token("|", parent))
		child = sequence_tokenizer("|", parent);
	return (child);
}
