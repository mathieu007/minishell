/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/21 15:12:56 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_token_pipe(t_token *token)
{
	while (token)
	{
		if (token->type == TK_PIPE)
			return (true);
		token = token->next;
	}
	return (false);
}

t_token	*create_cmds(t_token *token, t_cmd *parent)
{
	if (!token)
		return (NULL);
	if (has_token_semicolon(token))
		token = add_cmds_sequential(token, parent);
	else if (has_token_or(token))
		token = add_cmds_or(token, parent);
	else if (has_token_and(token))
		token = add_cmds_and(token, parent);
	else if (has_token_pipe(token))
		token = add_cmds_pipes(token, parent);
	else if (has_token_parenthese(token))
		token = add_cmd_parenthese(token, parent);
	else if (token->type == TK_CMD)
		token = add_cmd_execution(token, parent);
	else
		ft_printf("Something wrong happen!\n");
	return (token);
}
