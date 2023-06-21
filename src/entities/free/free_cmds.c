#include "minishell.h"

void	*free_t_tokens(t_token *token)
{
	t_token	*current;
	t_token	*next;

	current = token;
	while (current != NULL)
	{
		next = current->next;
		if (current->str)
			current->str = free_ptr(current->str);
		if (current->token_str)
			current->token_str = free_ptr(current->token_str);
		if (current->child)
			current->child = free_t_tokens(current->child);
		free(current);
		current = next;
	}
	return (NULL);
}

void	*free_t_cmd(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd;
	while (current != NULL)
	{
		next = current->next;
		if (current->child != NULL)
			current->child = free_t_cmd(current->child);
		current->name = free_ptr(current->name);
		current->full_path_name = free_ptr(current->full_path_name);
		current->args = free_2d_char_array(current->args);
		current->options = free_2d_array((void **)current->options);
		current->pipe = free_ptr(current->pipe);
		current->out_redir = free_t_redirect(current->out_redir);
		current->in_redir = free_t_redirect(current->in_redir);
		free(current);
		current = next;
	}
	return (NULL);
}

void	*free_t_process(t_process *proc)
{
	if (proc)
	{
		if (proc->cwd)
			proc->cwd = free_ptr(proc->cwd);
		if (proc->env_cpy)
			proc->env_cpy = free_t_env_cpy(proc->env_cpy);
		if (proc->tokens)
			proc->tokens = free_t_tokens(proc->tokens);
		if (proc->cmds)
			proc->cmds = free_t_cmd(proc->cmds);
		proc->last_error = free_ptr(proc->last_error);
	}
	return (NULL);
}