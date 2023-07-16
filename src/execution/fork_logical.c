#include "minishell.h"

int32_t	exec_logical_or(t_cmd *or_cmd)
{
	t_process	*proc;
	t_cmd		*child;
	t_cmd_seq	type;

	proc = get_process();
	if (proc->errnum == 0)
		proc->errnum = 1;
	while (or_cmd)
	{
		child = or_cmd->child;
		type = child->type;
		if (proc->errnum > 0 && (type == CMD_PARENTHESES || type == CMD))
			proc->errnum = exec_commands(child, false);
		else if (proc->errnum > 0)
			proc->errnum = exec_commands(child, false);
		else if (proc->errnum == 0 && child->next)
			proc->errnum = exec_commands(child->next, false);
		or_cmd = or_cmd->next;
	}
	return (proc->errnum);
}

int32_t	exec_logical_and(t_cmd *and_cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	proc->stop_exec = false;
	while (and_cmd && proc->errnum == 0)
	{
		proc->errnum = exec_commands(and_cmd->child, false);
		if (proc->errnum > 0)
			return (proc->errnum);
		and_cmd = and_cmd->next;
	}
	return (proc->errnum);
}
