/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_logical.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/04 08:35:53 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd_or(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	cmd = new_cmd(parent);
	cmd->type = CMD_LOG_OR;
	cmd->token = token;
	cmd->name = ft_strdup(token->str);
	if (token->child)
		create_cmds(token->child, cmd);
	return (cmd);
}

t_token	*add_cmds_or(t_token *token, t_cmd *parent)
{
	if (!parent || !token)
		return (NULL);
	create_cmd_or(token, parent);
	token = token->next;
	while (token->type == TK_OR)
	{
		create_cmd_or(token, parent);
		token = token->next;
	}
	return (token);
}

t_cmd	*create_cmd_and(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	cmd = new_cmd(parent);
	cmd->type = CMD_LOG_AND;
	cmd->token = token;
	cmd->name = ft_strdup(token->str);
	if (token->child)
		create_cmds(token->child, cmd);
	return (cmd);
}

t_token	*add_cmds_and(t_token *token, t_cmd *parent)
{
	if (!parent || !token)
		return (NULL);
	create_cmd_and(token, parent);
	token = token->next;
	while (token->type == TK_AND)
	{
		create_cmd_and(token, parent);
		token = token->next;
	}
	return (token);
}
