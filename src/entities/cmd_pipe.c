/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/04 08:38:23 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd_pipe(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	cmd = new_cmd(parent);
	cmd->type = CMD_PIPE;
	cmd->token = token;
	if (token->child)
		create_cmds(token->child, cmd);
	return (cmd);
}

t_token	*add_cmds_pipes(t_token *token, t_cmd *parent)
{
	if (!parent || !token)
		return (NULL);
	create_cmd_pipe(token, parent);
	token = token->next;
	while (token->type == TK_PIPE)
	{
		create_cmd_pipe(token, parent);
		token = token->next;
	}
	return (token);
}
