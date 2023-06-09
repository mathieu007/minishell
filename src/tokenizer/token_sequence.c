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
		token->str = ft_substr(str, start, len);
		token->child = dispatch_tokenizer(token);
		token = token->next;
	}
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

int32_t	add_sequence_token(int32_t i, char *tk_str, t_token_type type,
		t_token *parent)
{
	int32_t	len;
	char	*str;

	len = ft_strlen(tk_str);
	str = &parent->str[i + len];
	check_syntax_error_near(str, "|&;)}!");
	if (is_continuation(str))
	{
		exec_continuation(parent);
		return (add_sequence_token(i, tk_str, type, parent));
	}
	add_tk(tk_str, type, i, parent);
	i += len;
	return (i);
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
	add_sequence_token(i, "", TK_START, parent);
	while (parent->str[i])
	{
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
	if (has_token(";", parent))
		child = sequence_tokenizer(";", parent);
	else if (has_token("||", parent))
		child = sequence_tokenizer("||", parent);
	else if (has_token("&&", parent))
		child = sequence_tokenizer("&&", parent);
	else if (has_token("|", parent))
		child = sequence_tokenizer("|", parent);
	return (child);
}
