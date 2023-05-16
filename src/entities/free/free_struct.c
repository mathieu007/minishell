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
	if (data->str_cmds)
		free(data->str_cmds);
	if (data->argv[0] != NULL)
		free_2d_Array(data->argv);
	//TO FREE OR NOT TO FREE ?
	// if(data->env)
	//     free_2d_Array(data->argv);
	if (data->env_cpy)
		free_t_env_cpy(data->env_cpy);
	if (data->paths)
		free_2d_Array(data->paths);
	if (data->tokens)
		free_t_token(data->tokens);
	if (data->token_groups)
		free_t_token_group(data->token_groups);
	if (data->cmds)
		free_t_cmd(data->cmds);
	if (data->last_cmd)
		free_t_cmd(data->last_cmd);
	if (data->last_token_group)
		free_t_token(data->last_token_group);
	free(data);
}