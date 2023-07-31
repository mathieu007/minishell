/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/31 13:57:47 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_token_value(t_process *proc, t_token *child)
{
	char	*val;

	val = NULL;
	if (child->type == TK_COMMANDSUBSTITUTION_OPEN)
		val = ft_strdup(child->str);
	else if (child->type == TK_ENVIRONEMENT_VAR && ft_strisempty(child->str))
		val = ft_strdup("$");
	else if (child->type == TK_ENVIRONEMENT_VAR && child->str[0] == ' ')
		val = ft_strjoinfree(val, child->str);
	else if (child->type == TK_ENVIRONEMENT_VAR
		&& child->parent->type == TK_LESSLESS)
		val = ft_strjoin("$", child->str);
	else if (child->type == TK_DOLLAR_SIGN_CURLYBRACE
		&& child->parent->type == TK_LESSLESS)
	{
		val = ft_strjoin("${", child->str);
		val = ft_strjoinfree(val, "}");
	}
	else if (child->type == TK_ENVIRONEMENT_VAR
		|| child->type == TK_DOLLAR_SIGN_CURLYBRACE)
		val = parse_env_var_value(child);
	else if (child->type == TK_LAST_CMD_EXIT)
		val = ft_itoa(proc->last_errnum);
	else
		val = ft_strdup(child->str);
	return (val);
}

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
		val = process_token_value(proc, child);
		str = ft_strjoinfree2(str, val);
		child = child->next;
	}
	token->child = free_t_tokens(token->child);
	token->str = free_ptr(token->str);
	token->str = str;
	token->last = NULL;
	return (space_quotes_tokenizer(token));
}

// void	*build_token_environement(t_token *token)
// {
// 	char		*val;
// 	char		*str;
// 	t_process	*proc;
// 	t_token		*child;

// 	proc = get_process();
// 	str = NULL;
// 	if (token->type == TK_PARENTHESE_OPEN)
// 		return (token);
// 	if (!has_token_expansion(token))
// 		return (space_quotes_tokenizer(token));
// 	child = token->child;
// 	while (child && child->next)
// 	{
// 		if (child->type == TK_COMMANDSUBSTITUTION_OPEN)
// 			val = ft_strdup(child->str);
// 		else if (child->type == TK_ENVIRONEMENT_VAR &&
// 			ft_strisempty(child->str))
// 			val = ft_strdup("$");
// 		else if (child->type == TK_ENVIRONEMENT_VAR
// 			&& child->parent->type == TK_LESSLESS)
// 			val = ft_strjoin("$", child->str);
// 		else if (child->type == TK_DOLLAR_SIGN_CURLYBRACE
// 			&& child->parent->type == TK_LESSLESS)
// 		{
// 			val = ft_strjoin("${", child->str);
// 			val = ft_strjoinfree(val, "}");
// 		}
// 		else if (child->type == TK_ENVIRONEMENT_VAR
// 			|| child->type == TK_DOLLAR_SIGN_CURLYBRACE)
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
// 	token->str = str;
// 	token->last = NULL;
// 	return (space_quotes_tokenizer(token));
// }
