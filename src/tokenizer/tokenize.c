#include "minishell.h"

t_token	*add_tk_malloc(char *token_str, t_token_type type, int32_t i,
		t_token *parent)
{
	t_token	*token;

	token = add_token(i, type, parent);
	token->token_len = ft_strlen(token_str);
	// token->token_str = free_ptr(token->token_str);
	token->token_str = token_str;
	token->end = i + token->token_len;
	return (token);
}

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
	dispatch_tokenizer(root);
	return (proc->tokens);
}
