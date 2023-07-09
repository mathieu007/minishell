/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/05 09:09:25 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*create_cmd_execution(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	if (!parent)
		return (NULL);
	cmd = new_cmd(parent);
	cmd->type = CMD;
	cmd->token = token;
	return (cmd);
}

t_token	*add_cmd_execution(t_token *token, t_cmd *parent)
{
	if (!parent || !token)
		return (NULL);
	create_cmd_execution(token, parent);
	create_cmd_redirections(token, parent);
	return (token);
}
