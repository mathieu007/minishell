/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_sequential.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/20 10:29:05 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*create_cmd_sequential(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	cmd = new_cmd(parent);
	cmd->type = CMD_PARENTHESES;
	cmd->token = token;
	if (token->child)
		create_cmds(token->child, cmd);
	return (cmd);
}

t_token	*add_cmds_sequential(t_token *token, t_cmd *parent)
{
	t_cmd	*group;
	t_cmd	*cmd;

	if (!parent || !token)
		return (NULL);
	group = new_cmd(parent);
	group->type = CMD_GROUP_SEQUENTIAL;
	group->token = token;
	create_redirection(token, group);
	cmd = create_cmd_sequential(token, group);
	token = token->child;
	create_redirection(token, cmd);
	token = token->next;
	while (token->type == TK_SEMICOLON)
	{
		cmd = create_cmd_sequential(token, cmd);
		create_redirection(token, cmd);
		token = token->next;
	}
	return (token);
}
