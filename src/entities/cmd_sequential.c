/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_sequential.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/06 08:22:59 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*create_cmd_sequential(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	cmd = new_cmd(parent);
	cmd->type = CMD_SEQUENTIAL;
	cmd->token = token;
	if (token->child)
		create_cmds(token->child, cmd);
	return (cmd);
}

t_token	*add_cmds_sequential(t_token *token, t_cmd *parent)
{
	if (!parent || !token)
		return (NULL);
	create_cmd_sequential(token, parent);
	token = token->next;
	while (token->type != TK_END)
	{
		create_cmd_sequential(token, parent);
		token = token->next;
	}
	return (token);
}
