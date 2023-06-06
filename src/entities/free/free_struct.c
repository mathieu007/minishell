#include "minishell.h"

void	*free_t_redirect(t_redirect *redirect)
{
	if (!redirect)
		return (NULL);
	redirect->file_in = free_ptr(redirect->file_in);
	redirect->file_out = free_ptr(redirect->file_out);
	free(redirect);
	return (NULL);
}

void	free_t_data(t_process *data)
{
	if (data->tokens)
		free_t_tokens(data->tokens);
	free(data);
}