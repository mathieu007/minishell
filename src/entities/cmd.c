/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/14 14:50:19 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd()
{
	t_cmd	*new;

	new = ft_calloc(1, sizeof(t_cmd));
	new->pipe = NULL;
	if (new == NULL)
		return (NULL);
	return (new);
}

t_cmd	*goto_last_child_cmd(t_cmd *parent)
{
	t_cmd *child;

	if (!parent)
		return (NULL);
	child = parent->child;	
	while (child && child->next)
		child = child->next;
	return (child);
}

t_cmd	*goto_last_cmd(t_cmd *cmd)
{
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
}

inline t_cmd	*add_child_cmd(t_cmd *parent, t_token *token)
{
	t_cmd		*last_child;
	t_cmd		*new;

	if (!parent)
		return (NULL);
	last_child = goto_last_child_cmd(parent);
	new = new_cmd();
	new->cmd_seq_type = get_sequence_type(token);
	new->is_redirection = token->is_redirection;
	new->token = token;
	if (new == NULL)
		return (NULL);
	if (!parent->child)
	{
		parent->child = new;
		last_child = new;
	}		
	else
	{
		last_child->next = new;
		new->prev = last_child;
	}
	return (new);
}

t_cmd	*add_root_cmd_token(t_token *token)
{
	t_cmd		*last;
	t_cmd		*new;
	t_process	*data;

	data = get_process();
	last = data->last_cmd;
	new = new_cmd();
	new->cmd_seq_type = token->cmd_seq_type;
	new->is_redirection = token->is_redirection;
	new->token = token;
	if (new == NULL)
		return (NULL);
	if (!last)
		data->cmds = new;
	else
	{	
		last->next = new;
		new->prev = last;
	}
	data->last_cmd = new;
	return (new);
}

t_cmd	*add_cmd()
{
	t_cmd		*last;
	t_cmd		*new;
	t_process	*data;

	data = get_process();
	last = data->last_cmd;
	new = new_cmd();
	if (new == NULL)
		return (NULL);
	if (!last)
		data->cmds = new;
	else
	{	
		last->next = new;
		new->prev = last;
	}
	data->last_cmd = new;
	return (new);
}

void	*free_cmd(t_cmd *cmd)
{
	free_pipe(cmd);
	free_redirect(cmd);
	return (NULL);
}
