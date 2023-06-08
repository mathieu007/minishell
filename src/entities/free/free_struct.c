#include "minishell.h"

void	*free_t_redirect(t_redirect *redirect)
{
	if (!redirect)
		return (NULL);
	redirect->file = free_ptr(redirect->file);
	redirect->file = free_ptr(redirect->file);
	free(redirect);
	return (NULL);
}

void	free_t_data(t_process *data)
{
	if (data->tokens)
		free_t_tokens(data->tokens);
	data->tokens = NULL;
	if (data->cmds)
		free_t_cmd(data->cmds);
	data->cmds = NULL;
	data->last_cmd = NULL;
}