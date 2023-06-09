// // #include "minishell.h"

// static int32_t	add_other_token(char *token_str, t_token_type type,
// 	int32_t i, t_token_sequence *group)
// {
// 	t_token			*token;

// 	token = add_token(i, type, group);
// 	token->token_len = ft_strlen(token_str);
// 	token->token_str = token_str;
// 	token->end = i + token->token_len;
// 	return (token->end);
// }

// static int32_t	add_token_parenthese_open(int32_t i, t_token_type type, char *tk_str,
// 	t_token_sequence *group)
// {
// 	t_token			*token;

// 	token = add_token(i, type, group);
// 	token->token_len = 1;
// 	token->token_str = ft_strdup(tk_str);
// 	token->end = i + 1;
// 	return (token->end);
// }

// int32_t	tokenize_parentheses(char *str, int32_t i, t_token_sequence *group)
// {
// 	t_token_type	type;
// 	int32_t			t_len;
// 	int32_t			open_count;
// 	int32_t			close_count;

// 	open_count = 0;
// 	close_count = 0;
// 	i = add_token_parenthese(i, TK_PARENTHESE_OPEN, group);
// 	open_count = 1;
// 	while (str[i])
// 	{
// 		type = get_token_type(&str[i]);
// 		t_len = get_token_len(&str[i], type, true);
// 		if (type == TK_PARENTHESE_CLOSE)
// 		{
// 			close_count++;
// 			if (open_count == close_count)
// 				return (add_token_parenthese(i, TK_PARENTHESE_CLOSE, group));
// 		}
// 		else
// 			i += t_len;
// 	}
// 	return (i);
// }
