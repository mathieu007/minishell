#include "minishell.h"

void	*free_t_tokens(t_token *token)
{
	t_token	*current;
	t_token	*next;

	current = token;
	while (current != NULL)
	{
		next = current->next;
		if(current->str)
			current->str = free_ptr(current->str);
		if(current->token_str)
			current->token_str = free_ptr(current->token_str);
		if(current->child_tokens)
			current->child_tokens = free_t_tokens(current->child_tokens);
		free(current);
		current = next;
	}
	return(NULL);
}

void	*free_t_cmd(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd;
	while (current != NULL)
	{
		next = current->next;
		if(cmd->child != NULL)
		cmd->child = free_t_cmd(cmd->child);
		cmd->name = free_ptr(cmd->name);
		cmd->full_path_name = free_ptr(cmd->full_path_name);
		cmd->args = free_2d_char_array(cmd->args);
		cmd->options = free_2d_array((void **)cmd->options);
		cmd->pipe = free_ptr(cmd->pipe);
		cmd->out_redir = free_t_redirect(cmd->out_redir);
		cmd->out_redir = free_t_redirect(cmd->in_redir);
		free(current);
		current = next;
	}
	return (NULL);
}

void	*free_t_process(t_process *proc)
{
	if(proc)
	{
		if(proc->cwd)
		proc->cwd = free_ptr(proc->cwd);
		if(proc->env_cpy)
		proc->env_cpy = free_t_env_cpy(proc->env_cpy);
		if(proc->tokens)
		proc->tokens = free_t_tokens(proc->tokens);
		if(proc->env_cpy)
		proc->cmds = free_t_cmd(proc->cmds);
	}
		return(NULL);
}