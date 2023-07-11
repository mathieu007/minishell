#include "minishell.h"

int32_t	exec_sequential(t_cmd *seq)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	proc->stop_exec = false;
	if (!seq)
		return (proc->errnum);
	while (seq)
	{
		proc->errnum = exec_commands(seq->child, false);
		seq = seq->next;
	}
	return (proc->errnum);
}
