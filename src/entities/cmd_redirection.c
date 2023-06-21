/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/20 10:55:22 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd_seq	get_cmd_redir(t_token *token)
{
	if (!token)
		return (CMD_NONE);
	if (token->type == TK_GREAT)
		return (CMD_FILEOUT);
	else if (token->type == TK_GREATGREAT)
		return (CMD_FILEOUT_APPPEND);
	else if (token->type == TK_LESS)
		return (CMD_FILEIN);
	else if (token->type == TK_LESSLESS)
		return (CMD_HEREDOC);
	return (CMD_NONE);
}

static t_cmd	*create_cmd_redirection(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	cmd = new_cmd(parent);
	cmd->type = get_cmd_redir(token);
	cmd->token = token;
	return (cmd);
}

t_token	*add_cmds_redirections(t_token *token, t_cmd *parent)
{
	t_cmd	*group;

	if (!parent || !token)
		return (NULL);
	group = new_cmd(parent);
	group->type = CMD_GROUP_REDIRECTION;
	group->token = token;
	create_cmd_redirection(token, group);
	token = token->next;
	while (is_token_redir(token->type))
	{
		create_cmd_redirection(token, group);
		token = token->next;
	}
	return (token);
}
