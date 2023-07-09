/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parenthese.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/07 12:47:24 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd_parenthese(t_token *token, t_cmd *parent)
{
	t_cmd	*cmd;

	cmd = new_cmd(parent);
	cmd->type = CMD_PARENTHESES;
	cmd->token = token;
	if (token->child)
		create_cmds(token->child, cmd);
	return (cmd);
}

void	create_cmd_redirections(t_token *token, t_cmd *cmd)
{
	while (token && !is_token_redirection(token->type))
		token = token->next;
	if (token && is_token_redirection(token->type))
	{
		add_cmds_redirections(token, cmd);
		cmd->has_redirection = true;
		cmd->in_redir = free_t_redirect(cmd->in_redir);
		cmd->out_redir = free_t_redirect(cmd->out_redir);
		cmd->in_redir = new_redirect();
		cmd->out_redir = new_redirect();
	}
}

t_token	*add_cmd_parenthese(t_token *token, t_cmd *parent)
{
	if (!parent || !token)
		return (NULL);
	create_cmd_parenthese(token, parent);
	create_cmd_redirections(token, parent);
	return (token);
}
