
#include "minishell.h"

void	*free_ptr(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

void	free_all(void)
{
	t_process 		*proc;

	proc = get_process();
	printf("\n\n\n !!!  YOU USE FREE ALL !!! \n\n\n");

	free_t_process (proc);
}

void	*free_all_and_exit(int32_t status)
{	
	free_all();
	exit(status);
	return (NULL);
}

void	free_all_and_exit2(int32_t status, char *msg)
{
	perror(msg);
	free_all();
	exit(status);
}
