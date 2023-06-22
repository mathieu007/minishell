#include "minishell.h"

t_token	*tokenize_dbl_quotes_tokens(t_token *parent)
{
	int32_t			i;
	t_token_type	type;
	int32_t			t_len;
	char			*str;
	t_token			*start_token;

	i = 0;
	str = parent->str;
	start_token = add_tk("", TK_START, 0, parent);
	while (str[i])
	{
		type = get_token_type(&str[i]);
		t_len = get_token_len(&str[i], type, false);
		if (type == TK_COMMANDSUBSTITUTION_OPEN)
			i = add_token_substitution(str, i, parent, true);
		else if (type == TK_ENVIRONEMENT_VAR)
			i = add_token_env(str, i, parent, true);
		else
			i += t_len;
	}
	add_tk("", TK_END, i, parent);
	split_tokens(parent);
	return (start_token);
}
