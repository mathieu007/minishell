/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/20 10:27:56 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*create_cmd_execution(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	cmd = new_cmd(parent);
	cmd->type = CMD;
	cmd->token = token;
	return (cmd);
}

t_token	*add_cmd_execution(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	if (!parent || !token)
		return (NULL);
	cmd = create_cmd_execution(token, parent);
	create_redirection(token, cmd);
	return (token->next);
}
