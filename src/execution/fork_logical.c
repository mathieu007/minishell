#include "minishell.h"

// static int32_t	exec(t_cmd *cmd)
// {
// 	t_process	*proc;

// 	proc = get_process();
// 	if (proc->errnum > 0)
// 		return (proc->errnum);
// 	proc->errnum = cmd->func(cmd);
// 	return (proc->errnum);
// }

// int32_t	execute(t_cmd *cmd)
// {
// 	t_process	*proc;

// 	proc = get_process();
// 	proc->stop_exec = false;
// 	proc->errnum = 0;
// 	build_token_environement(cmd->token);
// 	cmd = parse_at_execution(cmd);
// 	create_fd_redir(cmd, cmd->next);
// 	if (cmd->is_builtin && cmd->next && !is_redirection(cmd->next->type))
// 		proc->errnum = exec(cmd);
// 	else if (proc->errnum == 0)
// 		fork_exec(cmd);
// 	return (proc->errnum);
// }

/// @brief logical operator such as && || stop the execution of the program
/// if an error occur.
/// @param cmd 
int32_t	exec_logical_or(t_cmd *group)
{
	t_process	*proc;
	t_cmd		*or_cmd;
	t_cmd		*cmd;

	or_cmd = group->child;
	proc = get_process();
	proc->errnum = 1;
	while (or_cmd && proc->errnum > 0)
	{
		cmd = or_cmd->child;
		proc->errnum = exec_commands(cmd, true);
		or_cmd = or_cmd->next;
	}
	return (proc->errnum);
}

// t_token	*contains_parentheses(t_token *token)
// {
// 	t_token	*child;

// 	child = token->child_tokens;
// 	while (child)
// 	{
// 		if (child->type == TK_PARENTHESE_OPEN)
// 			return (child);
// 		child = child->next;
// 	}
// 	return (NULL);
// }

int32_t	exec_logical_and(t_cmd *group)
{
	t_process	*proc;
	t_cmd		*and_cmd;

	and_cmd = group->child;
	proc = get_process();
	proc->errnum = 0;
	proc->stop_exec = false;
	while (and_cmd && proc->errnum == 0)
	{
		proc->errnum = exec_commands(and_cmd->child, true);
		if (proc->errnum > 0)
			return (proc->errnum);
		and_cmd = and_cmd->next;
	}
	return (proc->errnum);
}
