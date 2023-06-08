
#include "minishell.h"

void	*free_ptr(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

void	free_all(void)
{

	t_process 		*data;
	t_cmd 			*cmd;

	data = get_process();
	cmd = data->cmds;
	// if (token_group != NULL)
	// 	free_t_token_groups(token_group);
	if (cmd != NULL)
		free_t_cmd(cmd);
}

void	*free_all_and_exit(int32_t status)
{
	free_all();
	exit(status);
	return (NULL);
}
