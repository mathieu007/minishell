/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/13 13:53:09 by math             ###   ########.fr       */
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

void	create_by_type(t_token *token, t_cmd *parent, t_token_type type)
{
	while (token)
	{
		if (token->type == type)
			create_cmd_redirection(token, parent);
		token = token->next;
	}
}

t_token	*add_cmds_redirections(t_token *token, t_cmd *parent)
{
	if (!parent || !token)
		return (NULL);
	while (token && !is_token_redirection(token->type))
		token = token->next;
	while (token && is_token_redirection(token->type))
	{
		create_cmd_redirection(token, parent);
		token = token->next;
	}
	return (token);
}

// t_token	*add_cmds_redirections(t_token *token, t_cmd *parent)
// {
// 	if (!parent || !token)
// 		return (NULL);
// 	while (token && !is_token_redirection(token->type))
// 		token = token->next;
// 	create_by_type(token, parent, TK_LESSLESS);
// 	create_by_type(token, parent, TK_LESS);
// 	create_by_type(token, parent, TK_GREATGREAT);
// 	create_by_type(token, parent, TK_GREAT);
// 	return (token);
// }
