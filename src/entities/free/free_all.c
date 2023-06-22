
#include "minishell.h"

void	*free_ptr(void *ptr)
{
	if (ptr)
		free(ptr);	
	return (NULL);
}

void	free_all(void)
{
	t_process	*proc;

	proc = get_process();
	free_t_process(proc);
}

void	*free_all_and_exit(int32_t status)
{
	free_all();
	exit(status);
	return (NULL);
}

void	free_all_and_exit2(int32_t status, char *msg)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = status;
	perror(msg);
	free_all();
	exit(status);
}

void	free_exit_no_perr2(int32_t status, char *msg, char *msg2)
{
	t_process	*proc;

	proc = get_process();
	write_err2(status, msg, msg2);
	proc->errnum = status;
	free_all();
	exit(status);
}
