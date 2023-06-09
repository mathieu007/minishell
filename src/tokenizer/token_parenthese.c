#include "minishell.h"

static void	check_before_parenthese_syntax_error(char *str)
{
	int32_t	i;

	i = 0;
	while (str[i] && str[i] != '(')
	{
		if (str[i] != ' ')
			free_exit_no_perr2(2, "syntax error near unexpected token: ",
					&str[i]);
		i++;
	}
}

static void	check_after_parenthese_syntax_error(char *str)
{
	int32_t			i;
	t_token_type	type;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	type = get_token_type(&str[i]);
	if (str[i] != '\0' && !is_token_redirection(type))
		free_exit_no_perr2(2, "syntax error near unexpected token: ", &str[i]);
}

int32_t	add_token_parenthese(char *str, int32_t i, t_token *parent)
{
	int32_t	i_open;

	i_open = i;
	i = goto_closing_parenthese(str, i);
	if (str[i] != ')')
	{
		exec_delimiter_continuation(")", parent);
		return (add_token_parenthese(str, i_open, parent));
	}
	add_tk("(", TK_PARENTHESE_OPEN, i_open, parent);
	add_tk(")", TK_PARENTHESE_CLOSE, i, parent);
	i++;
	return (i);
}

void	split_token_parentheses(t_token *parent)
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
		if (is_token_redirection(token->type))
			token->child = expansion_tokenizer(token);
		token = token->next;
	}
}

int32_t	check_syntax_errors(t_token *parent)
{
	int32_t	i;

	i = 0;
	check_before_parenthese_syntax_error(parent->str);
	i = goto_token(parent->str, "(");
	check_after_parenthese_syntax_error(&parent->str[i + 1]);
	return (i);
}

t_token	*parentheses_tokenizer(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;

	if (!has_token("(", parent))
		return (NULL);
	i = check_syntax_errors(parent);
	i = add_token_parenthese(parent->str, i, parent);
	while (parent->str[i])
	{
		type = get_token_type(&parent->str[i]);
		t_len = get_token_len(&parent->str[i], type, false);
		if (is_token_redirection(type))
			i = add_token_redirection(parent->str, i, type, parent);
		else
			i += t_len;
	}
	while (parent->str[i] == ' ')
		i--;
	add_tk("", TK_END, i, parent);
	split_token_parentheses(parent);
	return (parent->child);
}
