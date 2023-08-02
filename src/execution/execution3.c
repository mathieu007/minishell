/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/02 08:07:54 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_before_execution(t_cmd *cmd)
{
	cmd = parse_cmd(cmd);
	if (!cmd)
		return (NULL);
	if (cmd->next && is_token_redirection(cmd->next->token->type))
		cmd->has_redirection = true;
	if (cmd->is_builtin)
		add_built_in_func(cmd);
	else
		add_execve_func(cmd);
	return (cmd);
}

void	reset_cmd(void)
{
	t_process	*proc;

	proc = get_process();
	free_t_cmd(proc->cmds);
	free_t_tokens(proc->tokens);
	proc->tokens = NULL;
	proc->cmds = NULL;
	proc->last_cmd = NULL;
	proc->signal = 0;
	proc->last_errnum = proc->errnum;
	proc->tokens = NULL;
	proc->execution = EXEC_END;
	proc->continuation = NULL;
	proc->syntax_error = false;
}

int32_t	exec_cmds(char *str)
{
	t_token		*token;
	t_cmd		*root_cmd;
	t_process	*proc;

	token = NULL;
	root_cmd = NULL;
	proc = get_process();
	proc->errnum = 0;
	if (!ft_strisempty(str) && !ft_striswhitespace(str))
		token = tokenize(str);
	root_cmd = create_cmds_tree(token);
	proc->signal = 0;
	if (root_cmd)
		dispatch_command(root_cmd->child, false);
	reset_cmd();
	return (proc->errnum);
}
