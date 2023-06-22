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

t_cmd_seq	get_cmd_seq_start(t_token *token)
{
	if (token->next->type == TK_PIPE)
		return (CMD_GROUP_PIPE);
	else if (token->next->type == TK_AND)
		return (CMD_GROUP_AND);
	else if (token->next->type == TK_OR)
		return (CMD_GROUP_OR);
	else if (token->next->type == TK_SEMICOLON)
		return (CMD_GROUP_SEQUENTIAL);
	return (CMD_NONE);
}

t_cmd_seq	get_cmd_seq(t_token *token)
{
	if (!token || !token->next)
		return (CMD_NONE);
	if (token->type == TK_START)
		return (get_cmd_seq_start(token));
	else if (is_token_redir(token->type))
		return (CMD_GROUP_REDIRECTION);
	else if (token->type == TK_PARENTHESE_OPEN)
		return (CMD_PARENTHESES);
	else if (token->type == TK_PIPE)
		return (CMD_GROUP_PIPE);
	else if (token->type == TK_AND && token->next
			&& token->next->type == TK_PIPE)
		return (CMD_GROUP_PIPE);
	else if (token->type == TK_AND)
		return (CMD_GROUP_AND);
	else if (token->type == TK_OR && token->next
			&& token->next->type == TK_PIPE)
		return (CMD_GROUP_PIPE);
	else if (token->type == TK_OR && token->next && token->next->type == TK_AND)
		return (CMD_GROUP_AND);
	else if (token->type == TK_OR)
		return (CMD_GROUP_OR);
	else if (token->type == TK_SEMICOLON)
		return (CMD_GROUP_SEQUENTIAL);
	else if (token->type == TK_CMD)
		return (CMD);
	return (CMD_NONE);
}

t_token	*create_cmds(t_token *token, t_cmd *parent)
{
	t_cmd_seq	seq;

	if (!token)
		return (NULL);
	while (token && token->type != TK_END)
	{
		seq = get_cmd_seq(token);
		if (!token->str[0] && seq == CMD_NONE)
		{
			token = token->next;
			continue ;
		}
		if (seq == CMD_GROUP_SEQUENTIAL)
			token = add_cmds_sequential(token, parent);
		else if (seq == CMD_GROUP_PIPE)
			token = add_cmds_pipes(token, parent);
		else if (seq == CMD_GROUP_AND)
			token = add_cmds_and(token, parent);
		else if (seq == CMD_GROUP_OR)
			token = add_cmds_or(token, parent);
		else if (seq == CMD_PARENTHESES)
			token = add_cmd_parenthese(token, parent);
		else if (seq == CMD)
			token = add_cmd_execution(token, parent);
		else if (seq == CMD_GROUP_REDIRECTION)
			token = add_cmds_redirections(token, parent);
		else
			printf("Something wrong happen!\n");
	}
	return (token);
}
