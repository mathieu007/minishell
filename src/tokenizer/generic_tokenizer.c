#include "minishell.h"

int32_t	add_space_token(char *str, int32_t i, t_token *parent)
{
	t_token_type	type;

	type = get_token_type(" ");
	add_tk(" ", type, i, parent);
	while (str[i] == ' ')
		i++;
	return (i);
}

inline bool	is_token_quotes(t_token_type type)
{
	return (type == TK_DOUBLEQUOTE || type == TK_SINGLEQUOTE);
}

inline bool	is_token_delimiter(t_token_type type)
{
	return (type == TK_DOUBLEQUOTE || type == TK_SINGLEQUOTE
		|| type == TK_PARENTHESE_OPEN || type == TK_COMMANDSUBSTITUTION_OPEN);
}

inline bool	is_token_group(t_token_type type)
{
	return (type == TK_DOUBLEQUOTE || type == TK_SINGLEQUOTE
		|| type == TK_ENVIRONEMENT_VAR || type == TK_PARENTHESE_OPEN
		|| type == TK_LAST_CMD_EXIT || type == TK_COMMANDSUBSTITUTION_OPEN);
}

inline int32_t	goto_token(char *str, char *tk)
{
	int32_t	i;
	int32_t	i2;
	int32_t	tk_len;

	i = 0;
	tk_len = ft_strlen(tk);
	while (str[i])
	{
		i2 = 0;
		while (i2 < tk_len && tk[i2] == str[i + i2])
		{
			i2++;
			if (i2 == tk_len)
				return (i);
		}
		i++;
	}
	return (-1);
}

int32_t	skip_token_single_quote(char *str, t_token_type type, int32_t i)
{
	if (type == TK_SINGLEQUOTE)
		i = goto_closing_single_quote(str, i + 1);
	return (i + 1);
}

int32_t	check_dbl_quotes_continuation(int32_t i, t_token *parent)
{
	i = goto_closing_double_quote(parent->str, i + 1);
	if (parent->str[i] != '\"')
	{
		exec_delimiter_continuation("\"", parent);
		return (goto_closing_double_quote(parent->str, i));
	}
	return (i);
}

int32_t	check_substitution_continuation(int32_t i, t_token *parent)
{
	i = goto_closing_parenthese(parent->str, i + 2);
	if (parent->str[i] != ')')
	{
		exec_delimiter_continuation(")", parent);
		return (goto_closing_parenthese(parent->str, i));
	}
	return (i);
}

int32_t	check_sgl_quotes_continuation(int32_t i, t_token *parent)
{
	i = goto_closing_single_quote(parent->str, i + 1);
	if (parent->str[i] != '\'')
	{
		exec_delimiter_continuation("\'", parent);
		return (goto_closing_single_quote(parent->str, i));
	}
	return (i);
}

int32_t	check_parenthese_continuation(int32_t i, t_token *parent)
{
	i = goto_closing_parenthese(parent->str, i + 1);
	if (parent->str[i] != ')')
	{
		exec_delimiter_continuation(")", parent);
		return (goto_closing_parenthese(parent->str, i));
	}
	return (i);
}

int32_t	skip_token_delimiter(t_token_type type, int32_t i, t_token *parent)
{
	if (type == TK_DOUBLEQUOTE)
		i = check_dbl_quotes_continuation(i, parent);
	else if (type == TK_SINGLEQUOTE)
		i = check_sgl_quotes_continuation(i, parent);
	else if (type == TK_COMMANDSUBSTITUTION_OPEN)
		i = check_substitution_continuation(i, parent);
	else if (type == TK_PARENTHESE_OPEN)
		i = check_parenthese_continuation(i, parent);
	return (i + 1);
}

// static int32_t	goto_closing_token(char *str, t_token_type type, int32_t i)
// {
// 	if (type == TK_DOUBLEQUOTE)
// 		i = goto_closing_double_quote(str, i + 1);
// 	else if (type == TK_SINGLEQUOTE)
// 		i = goto_closing_single_quote(str, i + 1);
// 	else if (type == TK_COMMANDSUBSTITUTION_OPEN)
// 		i = goto_closing_parenthese(str, i + 2);
// 	else if (type == TK_DOLLAR_SIGN)
// 		i = goto_closing_environement(str, i + 1);
// 	else if (type == TK_DOLLAR_SIGN_CURLYBRACE)
// 		i = goto_closing_environement(str, i + 1);
// 	else if (type == TK_LAST_CMD_EXIT)
// 		i++;
// 	else if (type == TK_PARENTHESE_OPEN)
// 		i = goto_closing_parenthese(str, i + 1);
// 	return (i);
// }

bool	has_token_redirection(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	if (get_process()->syntax_error)
		return (false);
	i = 0;
	while (parent->str[i])
	{
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == TK_GREAT || type == TK_GREATGREAT)
			return (true);
		else if (type == TK_LESS || type == TK_LESSLESS)
			return (true);
		else
			i += t_len;
	}
	return (false);
}

bool	has_token_semicolon_sequence(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	if (get_process()->syntax_error)
		return (false);
	i = 0;
	while (parent->str[i])
	{
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == TK_SEMICOLON)
			return (true);
		else
			i += t_len;
	}
	return (false);
}

bool	has_token_sequence(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	if (get_process()->syntax_error)
		return (false);
	i = 0;
	while (parent->str[i])
	{
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (is_token_delimiter(type))
			i = skip_token_delimiter(type, i, parent);
		else if (type == TK_OR || type == TK_AND || type == TK_PIPE)
			return (true);
		else
			i += t_len;
	}
	return (false);
}

bool	has_token(char *tk, t_token *parent)
{
	int32_t			i;
	t_token_type	cur_tk;
	int32_t			t_len;
	t_token_type	type;

	if (get_process()->syntax_error)
		return (false);
	i = 0;
	type = get_token_type(tk);
	while (parent->str[i])
	{
		cur_tk = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], cur_tk, false);
		if (type != cur_tk && is_token_delimiter(cur_tk))
			i = skip_token_delimiter(cur_tk, i, parent);
		else if (cur_tk == type)
			return (true);
		else
			i += t_len;
	}
	return (false);
}

t_token	*dispatch_tokenizer(t_token *parent)
{
	t_token	*child;

	if (has_error())
		return (NULL);
	child = sequences_tokenizer(parent);
	if (!child)
		child = parentheses_tokenizer(parent);
	if (!child)
		child = cmd_tokenizer(parent);
	return (child);
}
