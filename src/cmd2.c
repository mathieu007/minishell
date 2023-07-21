/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/06/26 15:41:18 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_token_and(t_token *token)
{
	while (token)
	{
		if (token->type == TK_AND)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_token_or(t_token *token)
{
	while (token)
	{
		if (token->type == TK_OR)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_token_semicolon(t_token *token)
{
	while (token)
	{
		if (token->type == TK_SEMICOLON)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_token_redirs(t_token *token)
{
	while (token)
	{
		if (token->type == TK_GREAT || token->type == TK_GREATGREAT
			|| token->type == TK_LESS || token->type == TK_LESSLESS)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_token_parenthese(t_token *token)
{
	while (token)
	{
		if (token->type == TK_PARENTHESE_OPEN)
			return (true);
		token = token->next;
	}
	return (false);
}
