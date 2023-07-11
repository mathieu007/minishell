#include "minishell.h"

int32_t	exec_logical_or(t_cmd *or_cmd)
{
	t_process	*proc;

	proc = get_process();
	if (proc->errnum == 0)
		proc->errnum = 1;
	while (or_cmd && proc->errnum > 0)
	{
		proc->errnum = exec_commands(or_cmd->child, true);
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
		proc->errnum = exec_commands(and_cmd->child, true);
		if (proc->errnum > 0)
			return (proc->errnum);
		and_cmd = and_cmd->next;
	}
	return (proc->errnum);
}
