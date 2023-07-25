/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 12:06:56 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int32_t	fork_exec(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	if (!cmd)
		return (proc->errnum);
	setup_child_signal_handlers(cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		enable_ctrl_c_output();
		return (exec_from_child_process(cmd));
	}
	reset_signal_handlers();
	proc->errnum = ft_waitpid(pid);
	proc->execution = EXEC_END;
	return (proc->errnum);
}

int32_t	build_cmd(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	if (!cmd->args)
	{
		build_token_environement(cmd->token);
		cmd = re_parse_at_execution(cmd);
		if (!cmd)
			return (-1);
		if (proc->errnum > 0)
			return (proc->errnum);
		if (cmd->has_redirection)
			create_fd_redir(cmd, cmd->next);
		return (proc->errnum);
	}
	return (proc->errnum);
}

int32_t	exec_command(t_cmd *cmd, bool is_in_child_process)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (proc->errnum == -1)
		return (proc->errnum);
	if (proc->errnum > 0 || ft_strisempty(cmd->name))
		return (proc->errnum);
	if (cmd->is_builtin && is_in_child_process)
		proc->errnum = exec_from_child_process(cmd);
	else if (cmd->is_builtin && proc->is_subshell)
		proc->errnum = exec_from_subshell_process(cmd);
	else if (cmd->is_builtin && !is_in_child_process)
		proc->errnum = exec_from_main_process(cmd);
	else if (is_in_child_process)
		proc->errnum = exec_from_child_process(cmd);
	else
		proc->errnum = fork_exec(cmd);
	if (proc->errnum <= 128)
		unlink_files_redirections(cmd->in_redir);
	return (proc->errnum);
}

int32_t	dispatch_command(t_cmd *cmd, bool is_in_child_process)
{
	t_process	*proc;

	proc = get_process();
	if (cmd->type == CMD_LOG_OR && proc->errnum == 0)
		proc->errnum = -1;
	if (cmd->type == CMD_LOG_AND && !cmd->prev)
		proc->errnum = 0;
	if (cmd->type == CMD_PIPE)
		proc->errnum = exec_pipes_cmds(cmd);
	else if (cmd->type == CMD_SEQUENTIAL)
		proc->errnum = exec_sequential(cmd);
	else if (cmd->type == CMD_LOG_AND && proc->errnum == 0)
		proc->errnum = exec_logical_and(cmd);
	else if (cmd->type == CMD_LOG_OR && proc->errnum > 0)
		proc->errnum = exec_logical_or(cmd);
	else if (cmd->type == CMD_PARENTHESES)
		proc->errnum = exec_subshell(cmd);
	else if (cmd->type == CMD)
	{
		proc->errnum = exec_command(cmd, is_in_child_process);
		proc->last_errnum = proc->errnum;
	}
	return (proc->errnum);
}

t_cmd	*create_cmds_tree(t_token *root_token)
{
	t_cmd		*root_cmd;
	t_process	*proc;
	t_token		*token;

	if (has_error() || !root_token)
		return (NULL);
	token = root_token->child;
	if (!token)
		return (NULL);
	proc = get_process();
	root_cmd = new_cmd(NULL);
	create_cmds(token, root_cmd);
	proc->cmds = root_cmd;
	return (root_cmd);
}
