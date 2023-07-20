/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/06/26 15:41:18 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*goto_last_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (NULL);
	while (cmd->next)
		cmd = cmd->next;
	return (cmd);
}

t_cmd	*new_cmd(t_cmd *parent)
{
	t_cmd	*new;
	t_cmd	*last;

	last = NULL;
	new = ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (free_all_and_exit2(errno, "malloc error"), NULL);
	if (!parent)
		return (new);
	if (!parent->child)
		parent->child = new;
	else
	{
		last = goto_last_cmd(parent->child);
		new->prev = last;
		last->next = new;
	}
	new->parent = parent;
	new->pipe = NULL;
	return (new);
}

t_cmd_seq	get_current_cmd_seq(t_token *token)
{
	if (!token)
		return (CMD_NONE);
	if (token->type == TK_END)
		return (get_current_cmd_seq(token->prev));
	else if (token->type == TK_PIPE)
		return (CMD_GROUP_PIPE);
	else if (token->type == TK_AND)
		return (CMD_GROUP_AND);
	else if (token->type == TK_OR)
		return (CMD_GROUP_OR);
	else if (token->type == TK_SEMICOLON)
		return (CMD_GROUP_SEQUENTIAL);
	return (CMD_NONE);
}

t_cmd_seq	get_next_cmd_seq(t_token *token)
{
	t_cmd_seq	curr_cmd_seq;

	curr_cmd_seq = get_current_cmd_seq(token);
	if (!token->next)
		return (curr_cmd_seq);
	else if (token->type == TK_START)
		return (get_current_cmd_seq(token->next));
	else if (token->next->type == TK_PIPE)
		return (CMD_GROUP_PIPE);
	else if (token->next->type == TK_AND && token->type != TK_PIPE)
		return (CMD_GROUP_AND);
	else if (token->next->type == TK_OR && token->type != TK_PIPE
		&& token->type != TK_AND)
		return (CMD_GROUP_OR);
	else if (token->next->type == TK_SEMICOLON)
		return (CMD_GROUP_SEQUENTIAL);
	return (curr_cmd_seq);
}

t_cmd_seq	get_cmd_seq(t_token *token)
{
	t_cmd_seq	next_seq;

	if (!token || !token->next)
		return (CMD_NONE);
	next_seq = get_next_cmd_seq(token);
	if (is_token_redirection(token->type))
		return (CMD_GROUP_REDIRECTION);
	else if (token->type == TK_PARENTHESE_OPEN)
		return (CMD_PARENTHESES);
	else if (token->type == TK_CMD)
		return (CMD);
	return (next_seq);
}

bool	has_token_and(t_token *token)
{
	while (token)
	{
		if (token->type == TK_AND)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_token_or(t_token *token)
{
	while (token)
	{
		if (token->type == TK_OR)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_token_semicolon(t_token *token)
{
	while (token)
	{
		if (token->type == TK_SEMICOLON)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_token_redirs(t_token *token)
{
	while (token)
	{
		if (token->type == TK_GREAT || token->type == TK_GREATGREAT
			|| token->type == TK_LESS || token->type == TK_LESSLESS)
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_token_parenthese(t_token *token)
{
	while (token)
	{
		if (token->type == TK_PARENTHESE_OPEN)
			return (true);
		token = token->next;
	}
	return (false);
}

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
		printf("Something wrong happen!\n");
	return (token);
}
