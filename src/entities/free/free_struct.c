#include "minishell.h"

void	free_t_redirect(t_redirect *redirect)
{
	if (redirect == NULL)
		return ;
	if (redirect->file_in != NULL)
		free(redirect->file_in);
	if (redirect->file_out != NULL)
		free(redirect->file_out);
	free(redirect);
}

void	free_t_data(t_data *data)
{
	if (data->env_cpy)
		free_t_env_cpy(data->env_cpy);
	if (data->paths)
		free_2d_array((void **)data->paths);
	if (data->tokens)
		free_t_tokens(data->tokens);
	if (data->token_groups)
		free_t_token_groups(data->token_groups);
	free(data);
}