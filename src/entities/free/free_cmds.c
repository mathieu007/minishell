#include "minishell.h"

void	free_t_cmd(t_cmd *cmd)
{
	t_cmd		*current;
	t_cmd		*next;
	t_process	*data;

	data = get_process();
	if (cmd == NULL)
		return ;
	current = cmd;

	while (current != NULL)
	{
		next = current->next;
		cmd->full_path_name = free_ptr(cmd->full_path_name);
		cmd->args = free_2d_char_array(cmd->args);
		cmd->options = free_2d_array((void **)cmd->options);
		cmd->pipe = free_ptr(cmd->pipe);
		cmd->out_redir = free_t_redirect(cmd->out_redir);
		free(current);
		current = next;
	}
	data->last_cmd = NULL;
	data->cmds = NULL;
}

void	free_t_token(t_token *token)
{
	token->str = free_ptr(token->str);
	free(token);
}

void	free_t_tokens(t_token *token)
{
	t_token	*current;
	t_token	*next;

	current = token;
	while (current != NULL)
	{
		next = current->next;
		current->str = free_ptr(current->str);
		free(current);
		current = next;
	}
}

void	free_t_env_cpy(t_env_cpy *env_cpy)
{
	t_env_cpy	*current;
	t_env_cpy	*next;

	current = env_cpy;
	while (current != NULL)
	{
		next = current->next;
		current->variable = free_ptr(current->variable);
		current->value = free_ptr(current->value);
		free(current);
		current = next;
	}
}

void	free_t_token_group(t_token_sequence *token_group)
{
	if (token_group)
	{
		token_group->str = free_ptr(token_group->str);
		// if (token_group->env_cpy)
		// 	free_t_env_cpy(token_group->env_cpy);
		if (token_group->token)
			free_t_tokens(token_group->token);
		token_group->token = NULL;
		token_group->last_token = NULL;
		free(token_group);
	}
}

void	free_t_token_groups(t_token_sequence *token_group)
{
	t_token_sequence	*next;
	// t_process 			*data;

	// data = get_process();
	while (token_group)
	{
		next = token_group->next;
		token_group->str = free_ptr(token_group->str);
		// if (token_group->env_cpy)
		// 	free_t_env_cpy(token_group->env_cpy);
		if (token_group->token)
			free_t_tokens(token_group->token);
		token_group->token = NULL;
		token_group->last_token = NULL;
		free(token_group);
		token_group = next;
	}
	// data->token_sequence = NULL;
	// data->last_token_sequence = NULL;
	// data->tokens_count = 0;
}