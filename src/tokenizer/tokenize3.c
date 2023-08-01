/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/31 20:40:03 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	substitution_syntax_error(void)
{
	char	*illegal_token;

	illegal_token = "Command substitution not handled.";
	write_err(258, illegal_token);
	return (-1);
}

int32_t	process_token_type(t_token *token, t_token_type type, int32_t i)
{
	if (type == TK_DOUBLEQUOTE)
		return (check_dbl_quotes_continuation(i, token));
	else if (type == TK_SINGLEQUOTE)
		return (check_sgl_quotes_continuation(i, token));
	else if (type == TK_COMMANDSUBSTITUTION_OPEN)
		return (substitution_syntax_error());
	else if (type == TK_DOLLAR_SIGN_CURLYBRACE)
		return (check_environement_continuation(i, token));
	else if (is_token_redirection(type))
		return (check_redirection_syntax_errors(token->str, type, i));
	else if (is_sequence_type(type))
		return (check_sequence_syntax_errors(i, type, token));
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
		i = process_token_type(token, type, i);
		if (i == -1)
			return ;
		if (token->str[i])
			i++;
	}
}

// void	check_continuations_and_error(t_token *token)
// {
// 	int32_t			i;
// 	t_token_type	type;

// 	i = 0;
// 	while (token->str[i])
// 	{
// 		type = get_token_type(&token->str[i]);
// 		if (type == TK_DOUBLEQUOTE)
// 			i = check_dbl_quotes_continuation(i, token);
// 		else if (type == TK_SINGLEQUOTE)
// 			i = check_sgl_quotes_continuation(i, token);
// 		else if (type == TK_COMMANDSUBSTITUTION_OPEN)
// 			i = substitution_syntax_error();
// 		else if (type == TK_PARENTHESE_OPEN)
// 			i = check_parenthese_continuation(i, token);
// 		else if (type == TK_DOLLAR_SIGN_CURLYBRACE)
// 			i = check_environement_continuation(i, token);
// 		else if (is_token_redirection(type))
// 			i = check_redirection_syntax_errors(token->str, type, i);
// 		else if (is_sequence_type(type))
// 			i = check_sequence_syntax_errors(i, type, token);
// 		if (i == -1)
// 			return ;
// 		if (token->str[i])
// 			i++;
// 	}
// }

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
